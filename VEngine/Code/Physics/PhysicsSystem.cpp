#include "vpch.h"
#include "PhysicsSystem.h"
#include <cassert>
#include <PxPhysicsAPI.h>
#include <PxRigidActor.h>
#include <characterkinematic/PxControllerManager.h>
#include "Components/CharacterControllerComponent.h"
#include "Components/MeshComponent.h"
#include "Components/DestructibleMeshComponent.h"
#include "Core/World.h"
#include "Asset/AssetSystem.h"
#include "Physics/Raycast.h"

using namespace physx;

//Store the original mesh UID against the new meshcomponent so that it can use its world matrix in render
std::unordered_map<UID, std::unique_ptr<MeshComponent>> physicsMeshes;

void NormaliseExtents(float& x, float& y, float& z);

//Maps meshcomponent UIDs to rigid actors
std::unordered_map<UID, PxRigidStatic*> rigidStaticMap;
std::unordered_map<UID, PxRigidDynamic*> rigidDynamicMap;

PxDefaultAllocator allocator;

//Need to link against PhysXExtensions_static_64.lib for this one. Also it needs to be from the debug folder.
PxDefaultErrorCallback errorCallback;

PxFoundation* foundation = nullptr;
PxPhysics* physics = nullptr;
PxDefaultCpuDispatcher* dispatcher = nullptr;
PxScene* scene = nullptr;
PxMaterial* material = nullptr;
PxMaterial* destructibleMaterial = nullptr;
PxPvd* pvd = nullptr;
PxControllerManager* controllerManager = nullptr;

void PhysicsSystem::Init()
{
	foundation = PxCreateFoundation(PX_PHYSICS_VERSION, allocator, errorCallback);
	assert(foundation);

	//nvidia physx debugger setup (pass pvd into PxCreatePhysics())
	//pvd = PxCreatePvd(*foundation);
	//PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	//pvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	physics = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, PxTolerancesScale(), true);
	assert(physics);

	dispatcher = PxDefaultCpuDispatcherCreate(2);
	assert(dispatcher);

	//Create scene
	PxSceneDesc sceneDesc(physics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	sceneDesc.cpuDispatcher = dispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	scene = physics->createScene(sceneDesc);

	//Default material
	material = physics->createMaterial(0.5f, 0.5f, 0.f);

	//Destructible material
	destructibleMaterial = physics->createMaterial(0.f, 0.f, 0.f);

	//Player capsule controller
	controllerManager = PxCreateControllerManager(*scene);
	assert(controllerManager);
}

//Setup physics actors on gameplay start
void PhysicsSystem::Start()
{
	auto actors = World::GetAllActorsInWorld();
	for (auto actor : actors)
	{
		auto meshes = actor->GetComponents<MeshComponent>();
		for (auto mesh : meshes)
		{
			auto dMesh = dynamic_cast<DestructibleMeshComponent*>(mesh);
			if (dMesh)
			{
				PhysicsSystem::CreatePhysicsForDestructibleMesh(dMesh);

				for (auto cell : dMesh->meshCells)
				{
					dMesh->RemoveChild(cell);
				}
			}
			else if (mesh && !mesh->skipPhysicsCreation)
			{
				if (mesh->UsesCollisonMesh())
				{
					const std::string collisionMeshFilename = mesh->GetCollisionMeshFilename();
					PhysicsSystem::CreateConvexPhysicsMeshFromCollisionMesh(mesh, actor, collisionMeshFilename);
				}
				else
				{
					PhysicsSystem::CreatePhysicsActor(mesh, mesh->GetPhysicsShape());
				}
			}
		}
	}
}

void PhysicsSystem::Tick(float deltaTime)
{
	//PxScene::simulate() complains if deltaTime is 0 or negative
	if (deltaTime <= 0.f) return;

	scene->simulate(deltaTime);
	scene->fetchResults(true);
}

void PhysicsSystem::Cleanup()
{
	scene->release();
	dispatcher->release();
	physics->release();

	//debugger shutdown
	if (pvd)
	{
		PxPvdTransport* transport = pvd->getTransport();
		pvd->release();
		pvd = nullptr;
		transport->release();
	}

	foundation->release();
}

void PhysicsSystem::Reset()
{
	for (auto& rigidActorIt : rigidDynamicMap)
	{
		rigidActorIt.second->release();
	}
	rigidDynamicMap.clear();

	for (auto& rigidActorIt : rigidStaticMap)
	{
		rigidActorIt.second->release();
	}
	rigidStaticMap.clear();
}

void PhysicsSystem::ReleasePhysicsActor(MeshComponent* mesh)
{
	PhysicsType physicsType =
		mesh->IsPhysicsStatic() ? physicsType = PhysicsType::Static : physicsType = PhysicsType::Dynamic;

	switch (physicsType)
	{
	case PhysicsType::Static:
	{
		auto rigidStaticIt = rigidStaticMap.find(mesh->GetUID());
		if (rigidStaticIt == rigidStaticMap.end())
		{
			return;
		}

		auto rigidStatic = rigidStaticIt->second;

		scene->removeActor(*rigidStatic);
		rigidStatic->release();
		rigidStaticMap.erase(mesh->GetUID());

		break;
	}
	case PhysicsType::Dynamic:
	{
		auto rigidDynamicIt = rigidDynamicMap.find(mesh->GetUID());
		if (rigidDynamicIt == rigidDynamicMap.end())
		{
			return;
		}

		auto rigidDynamic = rigidDynamicIt->second;

		scene->removeActor(*rigidDynamic);
		rigidDynamic->release();
		rigidDynamicMap.erase(mesh->GetUID());

		break;
	}
	}
}

void PhysicsSystem::CreatePhysicsActor(MeshComponent* mesh, const PhysicsActorShape physicsActorShape)
{
	PxTransform pxTransform;
	Transform transform;
	transform.Decompose(mesh->GetWorldMatrix());
	ActorToPhysxTransform(transform, pxTransform);

	const XMVECTOR extentsVector = mesh->GetBoundsExtents() * mesh->GetLocalScaleV();
	XMFLOAT3 extents;
	XMStoreFloat3(&extents, extentsVector);
	NormaliseExtents(extents.x, extents.y, extents.z);

	PxShape* shape = nullptr;

	switch (physicsActorShape)
	{
		case PhysicsActorShape::Box:
		{
			shape = physics->createShape(PxBoxGeometry(extents.x, extents.y, extents.z), *material);
			break;
		}
		case PhysicsActorShape::Sphere:
		{
			shape = physics->createShape(PxSphereGeometry(extents.x), *material);
			break;
		}
	}

	PhysicsType physicsType =
		mesh->IsPhysicsStatic() ? physicsType = PhysicsType::Static : physicsType = PhysicsType::Dynamic;

	switch (physicsType)
	{
	case PhysicsType::Static:
	{
		auto rigidStatic = physics->createRigidStatic(pxTransform);
		rigidStatic->userData = mesh;
		rigidStatic->attachShape(*shape);
		scene->addActor(*rigidStatic);
		//make sure neither static or dynamic actors are in the their opposite map
		assert(rigidDynamicMap.find(mesh->GetUID()) == rigidDynamicMap.end());
		rigidStaticMap.emplace(mesh->GetUID(), rigidStatic);
		break;
	}
	case PhysicsType::Dynamic:
	{
		auto rigidDynamic = physics->createRigidDynamic(pxTransform);
		rigidDynamic->userData = mesh;
		rigidDynamic->attachShape(*shape);
		scene->addActor(*rigidDynamic);
		assert(rigidStaticMap.find(mesh->GetUID()) == rigidStaticMap.end());
		rigidDynamicMap.emplace(mesh->GetUID(), rigidDynamic);
		break;
	}
	}
}

void PhysicsSystem::CreatePhysicsForDestructibleMesh(DestructibleMeshComponent* mesh)
{
	for (auto cell : mesh->meshCells)
	{
		CreateConvexPhysicsMesh(cell);
	}
}

void PhysicsSystem::CreateCharacterController(CharacterControllerComponent* characterControllerComponent)
{
	PxCapsuleControllerDesc desc = {};
	desc.height = characterControllerComponent->GetHeight();
	desc.radius = characterControllerComponent->GetRadius();
	desc.stepOffset = 0.3f;
	desc.volumeGrowth = 1.9f;
	desc.slopeLimit = cosf(XMConvertToRadians(15.f));
	desc.nonWalkableMode = PxControllerNonWalkableMode::ePREVENT_CLIMBING_AND_FORCE_SLIDING;
	desc.contactOffset = 0.5f;
	desc.upDirection = PxVec3(0.f, 1.f, 0.f);
	desc.material = material;

	auto pos = characterControllerComponent->GetWorldPositionV();
	desc.position.x = pos.m128_f32[0];
	desc.position.y = pos.m128_f32[1];
	desc.position.z = pos.m128_f32[2];

	PxController* controller = controllerManager->createController(desc);
	assert(controller);

	characterControllerComponent->SetController(controller);
}

//Todo: Convex physics bodies don't collide with anything. PhysX mesh is probably wrong. Use their debugger to check out.
//Also there's no split here between rigid and dynamic bodies. Fix that too once the above is fixed.
void PhysicsSystem::CreateConvexPhysicsMesh(MeshComponent* mesh)
{
	PxConvexMeshDesc convexDesc;
	convexDesc.points.count = mesh->meshDataProxy.GetVertices().size();
	convexDesc.points.stride = sizeof(Vertex);
	convexDesc.points.data = mesh->meshDataProxy.GetVertices().data();
	convexDesc.flags = PxConvexFlag::eCOMPUTE_CONVEX |
		PxConvexFlag::eDISABLE_MESH_VALIDATION | PxConvexFlag::eFAST_INERTIA_COMPUTATION;

	PxTolerancesScale scale;
	PxCookingParams params(scale);

	PxDefaultMemoryOutputStream buf;
	PxConvexMeshCookingResult::Enum result;
	if (!PxCookConvexMesh(params, convexDesc, buf, &result))
	{
		throw new std::exception("no cooking");
	}
	PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
	PxConvexMesh* convexMesh = physics->createConvexMesh(input);

	Transform transform;
	transform.Decompose(mesh->GetWorldMatrix());
	PxTransform pxTransform;
	ActorToPhysxTransform(transform, pxTransform);

	PxRigidDynamic* aConvexActor = physics->createRigidDynamic(pxTransform);
	auto geom = PxConvexMeshGeometry(convexMesh);
	PxRigidActorExt::createExclusiveShape(*aConvexActor, geom, *material);
	scene->addActor(*aConvexActor);

	rigidDynamicMap.emplace(mesh->GetUID(), aConvexActor);
}

void PhysicsSystem::CreateConvexPhysicsMeshFromCollisionMesh(MeshComponent* mesh,
	Actor* actor, const std::string filename)
{
	auto collisionMesh = new MeshComponent();

	auto actorTransform = actor->GetTransform();
	collisionMesh->SetTransform(actorTransform);

	//Set the UID to the actual mesh so that the physics actor is connected to the mesh, not the collision mesh.
	collisionMesh->SetUID(mesh->GetUID());

	collisionMesh->meshDataProxy = AssetSystem::ReadVMeshAssetFromFile(filename);
	collisionMesh->CreateVertexBuffer();

	CreateConvexPhysicsMesh(collisionMesh);

	assert(physicsMeshes.find(mesh->GetUID()) == physicsMeshes.end());
	physicsMeshes.emplace(collisionMesh->GetUID(), collisionMesh);
}

void PhysicsSystem::ActorToPhysxTransform(const Transform& actorTransform, PxTransform& pxTransform)
{
	pxTransform.p = PxVec3(actorTransform.position.x,
		actorTransform.position.y, actorTransform.position.z);

	//@Todo: does scale even work with Physx? right now physics actor scale is being set to 1x1x1 
	//as there's nothing set to it here, so any scale changes at runtime are NOT reflected.
	//Ref:https://gameworksdocs.nvidia.com/PhysX/4.0/documentation/PhysXGuide/Manual/Geometry.html#mesh-scaling

	pxTransform.q = PxQuat(actorTransform.rotation.x, actorTransform.rotation.y,
		actorTransform.rotation.z, actorTransform.rotation.w);
}

void PhysicsSystem::PhysxToActorTransform(Transform& actorTransform, const PxTransform& pxTransform)
{
	actorTransform.position = XMFLOAT3(pxTransform.p.x, pxTransform.p.y, pxTransform.p.z);
	actorTransform.rotation = XMFLOAT4(pxTransform.q.x, pxTransform.q.y, pxTransform.q.z, pxTransform.q.w);
}

void PhysicsSystem::GetTransformFromPhysicsActor(MeshComponent* mesh)
{
	PhysicsType physicsType =
		mesh->IsPhysicsStatic() ? physicsType = PhysicsType::Static : physicsType = PhysicsType::Dynamic;

	const auto uid = mesh->GetUID();
	Transform transform;

	switch (physicsType)
	{
	case PhysicsType::Static:
	{
		auto rigidStatic = rigidStaticMap.find(uid)->second;

		PxTransform pxTransform = rigidStatic->getGlobalPose();
		PhysxToActorTransform(transform, pxTransform);

		break;
	}
	case PhysicsType::Dynamic:
	{
		auto rigidDynamic = rigidDynamicMap.find(uid)->second;

		PxTransform pxTransform = rigidDynamic->getGlobalPose();
		PhysxToActorTransform(transform, pxTransform);

		break;
	}
	}

	mesh->SetWorldPosition(transform.position);
	mesh->SetWorldRotation(XMLoadFloat4(&transform.rotation));
}

void PhysicsSystem::SetTransformForPhysicsActor(MeshComponent* mesh)
{
	//Assert you're not hitting performance problems with static PhysX actors. 
	//See PxRigidActor::setGlobalPose() comments for more details.
	assert(!mesh->IsPhysicsStatic());

	const auto uid = mesh->GetUID();
	auto rigidDynamic = rigidDynamicMap.find(uid)->second;

	PxTransform transform;
	transform.p = PhysicsPhysx::XMVectorToPxVec3(mesh->GetWorldPositionV());
	transform.q = PhysicsPhysx::XMVectorToPxQuat(mesh->GetWorldRotationV());
	rigidDynamic->setGlobalPose(transform);
}

//Todo: is adding force not working?
void PhysicsSystem::AddForceToMesh(MeshComponent* mesh, XMVECTOR forceDirection)
{
	assert(!mesh->IsPhysicsStatic());

	const auto uid = mesh->GetUID();
	const auto rigid = rigidDynamicMap.find(uid)->second;
	const auto force = PhysicsPhysx::XMVectorToPxVec3(forceDirection);
	rigid->addForce(force);
}

std::unordered_map<UID, std::unique_ptr<MeshComponent>>& PhysicsSystem::GetAllPhysicsMeshes()
{
	return physicsMeshes;
}

bool PhysicsPhysx::Raycast(XMVECTOR origin, XMVECTOR direction, float range, HitResult& hitResult)
{
	const PxVec3 pxOrigin = XMVectorToPxVec3(origin);
	const PxVec3 pxDir = XMVectorToPxVec3(direction);

	PxRaycastBuffer hitBuffer;
	if (scene->raycast(pxOrigin, pxDir, range, hitBuffer))
	{
		const PxRaycastHit& block = hitBuffer.block;

		hitResult.hitComponents.push_back((MeshComponent*)block.actor->userData);
		hitResult.hitNormal = XMFLOAT3(block.normal.x, block.normal.y, block.normal.z);
		hitResult.hitPos = XMFLOAT3(block.position.x, block.position.y, block.position.z);
		hitResult.uv = XMFLOAT2(block.u, block.v);
		hitResult.origin = origin;
		hitResult.direction = direction;
		hitResult.range = range;

		return Physics::RaycastTriangleIntersect(hitResult);
	}

	return false;
}

PxVec3 PhysicsPhysx::XMVectorToPxVec3(XMVECTOR xmVector)
{
	return PxVec3(xmVector.m128_f32[0], xmVector.m128_f32[1], xmVector.m128_f32[2]);
}

PxQuat PhysicsPhysx::XMVectorToPxQuat(XMVECTOR xmVector)
{
	return PxQuat(xmVector.m128_f32[0], xmVector.m128_f32[1], xmVector.m128_f32[2], xmVector.m128_f32[3]);
}

PxVec3 PhysicsPhysx::Float3ToPxVec3(XMFLOAT3 float3)
{
	return PxVec3(float3.x, float3.y, float3.z);
}

XMFLOAT3 PhysicsPhysx::PxVec3ToFloat3(PxVec3 pxVec3)
{
	return XMFLOAT3(pxVec3.x, pxVec3.y, pxVec3.z);
}

bool PhysicsPhysx::BoxCast(XMFLOAT3 extents, XMFLOAT3 origin, XMFLOAT3 direction, float distance, HitResult& hitResult)
{
	const PxVec3 pxExtents = PhysicsPhysx::Float3ToPxVec3(extents);
	const PxVec3 pxDirection = PhysicsPhysx::Float3ToPxVec3(direction);

	//Use identity quaternion for sweep, won't need more than that for now.
	const PxTransform pose(PhysicsPhysx::Float3ToPxVec3(origin));

	PxSweepBuffer sweepBuffer;
	if (scene->sweep(PxBoxGeometry(pxExtents), PxTransform(), pxDirection, distance, sweepBuffer))
	{
		const PxSweepHit& block = sweepBuffer.block;

		hitResult.hitComponents.push_back((MeshComponent*)block.actor->userData);
		hitResult.hitNormal = XMFLOAT3(block.normal.x, block.normal.y, block.normal.z);
		hitResult.hitPos = XMFLOAT3(block.position.x, block.position.y, block.position.z);
		hitResult.origin = DirectX::XMLoadFloat3(&origin);
		hitResult.direction = DirectX::XMLoadFloat3(&direction);
		hitResult.range = distance;

		return Physics::RaycastTriangleIntersect(hitResult);
	}

	return false;
}

//Extents can be 0 or less than because of the planes and walls, Physx wants extents above 0.
void NormaliseExtents(float& x, float& y, float& z)
{
	if (x <= 0.f) x = 0.1f;
	if (y <= 0.f) y = 0.1f;
	if (z <= 0.f) z = 0.1f;
}
