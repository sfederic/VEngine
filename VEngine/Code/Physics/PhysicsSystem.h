#pragma once

#define PX_PHYSX_STATIC_LIB

#include <foundation/PxTransform.h>

using namespace physx;

struct MeshComponent;
struct Transform;

struct PhysicsSystem
{
	void Init();
	void Tick(float deltaTime);
	void Cleanup();

	void CreateRigidDynamicPhysicsActor(MeshComponent* mesh);
	void CreateRigidStaticPhysicsActor(MeshComponent* mesh);
	void ActorToPhysxTransform(const Transform& actorTransform, PxTransform& pxTransform);
	void PhysxToActorTransform(Transform& actorTransform, const PxTransform& pxTransform);
	void GetTransformFromPhysicsActor(MeshComponent* mesh);
};

extern PhysicsSystem physicsSystem;
