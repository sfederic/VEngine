#include "PhysicsSystem.h"
#include <cassert>
#include <PxPhysicsAPI.h>
#include "Components/MeshComponent.h"
#include <PxRigidActor.h>
#include "World.h"

PhysicsSystem physicsSystem;

PxDefaultAllocator allocator;

//Need to link against PhysXExtensions_static_64.lib for this one. Also it needs to be from the debug folder.
PxDefaultErrorCallback errorCallback;

PxFoundation* foundation = nullptr;
PxPhysics* physics = nullptr;
PxCooking* cooking = nullptr;
PxDefaultCpuDispatcher* dispatcher = nullptr;
PxScene* scene = nullptr;
PxMaterial* material = nullptr;
PxPvd* pvd = nullptr;

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

	//Create cooking objects
	cooking = PxCreateCooking(PX_PHYSICS_VERSION, *foundation, PxCookingParams(PxTolerancesScale()));
	assert(cooking);

	//Create scene
	PxSceneDesc sceneDesc(physics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	sceneDesc.cpuDispatcher = dispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	scene = physics->createScene(sceneDesc);

	//Default material
	material = physics->createMaterial(0.5f, 0.5f, 0.f);
}

//Setup physics actors on gameplay start
void PhysicsSystem::Start()
{
	//@Todo: it feels like physics will break if the mesh isn't the root of the actor
	auto actors = world.GetAllActorsInWorld();
	for (auto actor : actors)
	{
		auto meshes = actor->GetComponentsOfType<MeshComponent>();
		for (auto mesh : meshes)
		{
			//@Todo: I don't like doing this and reseting physicssystem on gameplay end. Feels expensive,
			//but it enables roughly changing actors between static and dynamic when editing.
			if (mesh->isStatic)
			{
				physicsSystem.CreatePhysicsActor(mesh, PhysicsType::Static, actor);
			}
			else
			{
				physicsSystem.CreatePhysicsActor(mesh, PhysicsType::Dynamic, actor);
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

//@Todo: for now, createphysicsactor functions are just using AABB boxes based on the DirectXMath bounding 
//volume vars. See if it's worth cooking collision hulls and how that weighs up with raycasting in Physx.
//Probably ok for grid games. VagrantTactics needs it more than collision hulls.
void PhysicsSystem::CreatePhysicsActor(MeshComponent* mesh, PhysicsType type, Actor* actor)
{
	PxTransform pxTransform = {};
	Transform transform = mesh->transform;
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

	rigidActor->userData = actor;

	XMFLOAT3 extents = mesh->boundingBox.Extents;
	NormaliseExtents(extents.x, extents.y, extents.z);
	auto box = physics->createShape(PxBoxGeometry(extents.x, extents.y, extents.z), *material);

	rigidActor->attachShape(*box);
	scene->addActor(*rigidActor);
	
	rigidActorMap.emplace(mesh->uid, rigidActor);
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
	auto rigid = rigidActorMap[mesh->uid];

	PxTransform pxTransform = rigid->getGlobalPose();
	Transform transform = mesh->transform;
	PhysxToActorTransform(transform, pxTransform);

	mesh->transform = transform;
	mesh->UpdateTransform();
}

//Extents can be 0 or less than because of the planes and walls, Physx wants extents above 0.
void PhysicsSystem::NormaliseExtents(float& x, float& y, float& z)
{
	if (x <= 0.f) x = 0.1f;
	if (y <= 0.f) y = 0.1f;
	if (z <= 0.f) z = 0.1f;
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
		hit.posiiton = XMFLOAT3(block.position.x, block.position.y, block.position.z);
		hit.uv = XMFLOAT2(block.u, block.v);

		return true;
	}

	return false;
}
