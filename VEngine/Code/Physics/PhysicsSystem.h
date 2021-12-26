#pragma once

#define PX_PHYSX_STATIC_LIB

#include <PxPhysicsAPI.h>

using namespace physx;

struct Actor;
struct Transform;

struct PhysicsSystem
{
	void Init();
	void Tick(float deltaTime);
	void Cleanup();

	void CreateRigidDynamicPhysicsActor(Actor* actor);
	void CreateRigidStaticPhysicsActor(Actor* actor);
	void ActorToPhysxTransform(const Transform& actorTransform, PxTransform& pxTransform);
	void PhysxToActorTransform(Transform& actorTransform, const PxTransform& pxTransform);
	void GetTransformFromPhysicsActor(Actor* actor);
};

extern PhysicsSystem physicsSystem;
