#include "PhysicsSystem.h"
#include <cassert>
#include <PxPhysicsAPI.h>
#include "Components/MeshComponent.h"
#include <PxRigidActor.h>

PhysicsSystem physicsSystem;
PxDefaultAllocator allocator;

//Need to link against PhysXExtensions_static_64.lib for this one. Also it needs to be from the debug folder.
PxDefaultErrorCallback errorCallback;

PxFoundation* foundation = nullptr;
PxPhysics* physics = nullptr;
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

	//Create scene
	PxSceneDesc sceneDesc(physics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -1.81f, 0.0f);
	sceneDesc.cpuDispatcher = dispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	scene = physics->createScene(sceneDesc);

	//Default material
	material = physics->createMaterial(0.5f, 0.5f, 0.f);
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
void PhysicsSystem::CreateRigidDynamicPhysicsActor(MeshComponent* mesh)
{
	PxTransform pxTransform = {};
	Transform transform = mesh->transform;
	ActorToPhysxTransform(transform, pxTransform);

	auto rigid = physics->createRigidDynamic(pxTransform);

	XMFLOAT3 extents = mesh->boundingBox.Extents;
	auto box = physics->createShape(PxBoxGeometry(extents.x, extents.y, extents.z), *material);

	rigid->attachShape(*box);
	scene->addActor(*rigid);
	
	rigidActorMap.emplace(mesh->uid, rigid);
}

void PhysicsSystem::CreateRigidStaticPhysicsActor(MeshComponent* mesh)
{
	PxTransform pxTransform = {};
	Transform actorTransform = mesh->transform;
	ActorToPhysxTransform(actorTransform, pxTransform);

	auto rigid = physics->createRigidStatic(pxTransform);

	XMFLOAT3 extents = mesh->boundingBox.Extents;
	auto box = physics->createShape(PxBoxGeometry(extents.x, extents.y, extents.z), *material);

	rigid->attachShape(*box);
	scene->addActor(*rigid);

	rigidActorMap.emplace(mesh->uid, rigid);
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
