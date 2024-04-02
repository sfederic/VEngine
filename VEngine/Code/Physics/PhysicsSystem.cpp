#include "vpch.h"
#include "PhysicsSystem.h"
#include <cassert>
#include <PxPhysicsAPI.h>
#include <characterkinematic/PxControllerManager.h>
#include "Components/CharacterControllerComponent.h"
#include "Components/MeshComponent.h"
#include "Components/DestructibleMeshComponent.h"
#include <PxRigidActor.h>
#include "Core/World.h"
#include "Asset/AssetSystem.h"

//Store the original mesh UID against the new meshcomponent so that it can use its world matrix in render
std::unordered_map<UID, std::unique_ptr<MeshComponent>> physicsMeshes;

void NormaliseExtents(float& x, float& y, float& z);

//Maps meshcomponent UIDs to rigid actors
std::unordered_map<UID, PxRigidActor*> rigidActorMap;

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
				PhysicsSystem::CreatePhysicsForDestructibleMesh(dMesh, actor);

				for (auto cell : dMesh->meshCells)
				{
					dMesh->RemoveChild(cell);
				}
			}
			else if (!mesh->skipPhysicsCreation)
			{
				PhysicsType physicsType;
				mesh->isPhysicsStatic ? physicsType = PhysicsType::Static : physicsType = PhysicsType::Dynamic;

				if (mesh->UsesCollisonMesh())
				{
					const std::string collisionMeshFilename = mesh->GetCollisionMeshFilename();
					PhysicsSystem::CreateConvexPhysicsMeshFromCollisionMesh(mesh, actor, collisionMeshFilename);
				}
				else
				{
					PhysicsSystem::CreatePhysicsActor(mesh, physicsType, actor);
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
	for (auto& rigidActorIt : rigidActorMap)
	{
		rigidActorIt.second->release();
	}

	rigidActorMap.clear();
}

void PhysicsSystem::ReleasePhysicsActor(MeshComponent* mesh)
{
	auto rigidActorIt = rigidActorMap.find(mesh->GetUID());
	if (rigidActorIt == rigidActorMap.end())
	{
		return;
	}

	auto rigidActor = rigidActorIt->second;

	scene->removeActor(*rigidActor);
	rigidActor->release();
	const auto meshUID = mesh->GetUID();
	rigidActorMap.erase(mesh->GetUID());
}

void PhysicsSystem::CreatePhysicsActor(MeshComponent* mesh, PhysicsType type, Actor* actor)
{
	PxTransform pxTransform;
	Transform transform;
	transform.Decompose(mesh->GetWorldMatrix());
	ActorToPhysxTransform(transform, pxTransform);

	PxRigidActor* rigidActor = nullptr;
	switch (type)
	{
	case PhysicsType::Static:
		rigidActor = physics->createRigidStatic(pxTransform);
		break;

	case PhysicsType::Dynamic:
		rigidActor = physics->createRigidDynamic(pxTransform);
		break;
	}

	//Set actor as user data
	assert(rigidActor);
	rigidActor->userData = actor;

	XMVECTOR extentsVector = mesh->GetBoundsExtents() * mesh->GetLocalScaleV();
	XMFLOAT3 extents;
	XMStoreFloat3(&extents, extentsVector);
	NormaliseExtents(extents.x, extents.y, extents.z);
	PxShape* box = physics->createShape(PxBoxGeometry(extents.x, extents.y, extents.z), *material);

	rigidActor->attachShape(*box);
	scene->addActor(*rigidActor);

	const auto meshUID = mesh->GetUID();
	//@Todo: some maps are hitting conflicts on this map. Sort them out then uncomment this assert.
	//assert(rigidActorMap.find(meshUID) == rigidActorMap.end());
	rigidActorMap.emplace(meshUID, rigidActor);
}

void PhysicsSystem::CreatePhysicsForDestructibleMesh(DestructibleMeshComponent* mesh, Actor* actor)
{
	for (auto cell : mesh->meshCells)
	{
		CreateConvexPhysicsMesh(cell, actor);
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

//@Todo: replace with physx 5.3 cooking code
void PhysicsSystem::CreateConvexPhysicsMesh(MeshComponent* mesh, Actor* actor)
{
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

	CreateConvexPhysicsMesh(collisionMesh, actor);

	assert(physicsMeshes.find(mesh->GetUID()) == physicsMeshes.end());
	physicsMeshes.emplace(collisionMesh->GetUID(), collisionMesh);
}

void PhysicsSystem::ActorToPhysxTransform(const Transform& actorTransform, PxTransform& pxTransform)
{
	pxTransform.p = PxVec3(actorTransform.position.x,
		actorTransform.position.y, actorTransform.position.z);

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
	const auto uid = mesh->GetUID();
	auto rigid = rigidActorMap.find(uid)->second;

	PxTransform pxTransform = rigid->getGlobalPose();
	Transform transform;
	PhysxToActorTransform(transform, pxTransform);

	mesh->SetTransform(transform);
}

std::unordered_map<UID, std::unique_ptr<MeshComponent>>& PhysicsSystem::GetAllPhysicsMeshes()
{
	return physicsMeshes;
}

bool Physics::Raycast(XMFLOAT3 origin, XMFLOAT3 dir, float range, RaycastHit& hit)
{
	PxRaycastBuffer hitBuffer;
	PxVec3 pxOrigin(origin.x, origin.y, origin.z);
	PxVec3 pxDir(dir.x, dir.y, dir.z);

	if (scene->raycast(pxOrigin, pxDir, range, hitBuffer))
	{
		PxRaycastHit& block = hitBuffer.block;

		hit.hitActor = (Actor*)block.actor->userData;
		hit.distance = block.distance;
		hit.normal = XMFLOAT3(block.normal.x, block.normal.y, block.normal.z);
		hit.position = XMFLOAT3(block.position.x, block.position.y, block.position.z);
		hit.uv = XMFLOAT2(block.u, block.v);

		return true;
	}

	return false;
}

PxVec3 Physics::Float3ToPxVec3(XMFLOAT3 float3)
{
	return PxVec3(float3.x, float3.y, float3.z);
}

XMFLOAT3 Physics::PxVec3ToFloat3(PxVec3 pxVec3)
{
	return XMFLOAT3(pxVec3.x, pxVec3.y, pxVec3.z);
}

bool Physics::BoxCast(XMFLOAT3 extents, XMFLOAT3 origin, XMFLOAT3 direction, float distance, RaycastHit& hit)
{
	PxVec3 pxExtents = Physics::Float3ToPxVec3(extents);
	PxVec3 pxDirection = Physics::Float3ToPxVec3(direction);

	//Use identity quaternion for sweep, won't need more than that for now.
	PxTransform pose(Physics::Float3ToPxVec3(origin));

	PxSweepBuffer sweepBuffer;
	if (scene->sweep(PxBoxGeometry(pxExtents), PxTransform(), pxDirection, distance, sweepBuffer))
	{
		PxSweepHit& block = sweepBuffer.block;

		hit.hitActor = (Actor*)block.actor->userData;
		hit.distance = block.distance;
		hit.normal = XMFLOAT3(block.normal.x, block.normal.y, block.normal.z);
		hit.position = XMFLOAT3(block.position.x, block.position.y, block.position.z);
		hit.uv = XMFLOAT2(0.f, 0.f); //No uv coords for sweeps

		return true;
	}

	return false;
}

std::vector<PxRigidActor*> PhysicsSystem::GetRigidActors()
{
	std::vector<PxRigidActor*> actors;
	for (const auto& [key, value] : rigidActorMap)
	{
		actors.emplace_back(value);
	}
	return actors;
}

//Extents can be 0 or less than because of the planes and walls, Physx wants extents above 0.
void NormaliseExtents(float& x, float& y, float& z)
{
	if (x <= 0.f) x = 0.1f;
	if (y <= 0.f) y = 0.1f;
	if (z <= 0.f) z = 0.1f;
}
