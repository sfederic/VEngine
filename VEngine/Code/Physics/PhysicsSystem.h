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

	void CreatePhysicsActor(Actor* actor);
	void ActorToPhysxTransform(Transform& actorTransform, PxTransform& pxTransform);
};

extern PhysicsSystem physicsSystem;
