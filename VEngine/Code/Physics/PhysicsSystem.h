#pragma once

#define PX_PHYSX_STATIC_LIB

#include <unordered_map>
#include <foundation/PxTransform.h>
#include <PxRigidActor.h>
#include "UID.h"

using namespace physx;

struct MeshComponent;
struct Transform;

struct PhysicsSystem
{
	//Maps meshcomponent UIDs to rigid actors
	std::unordered_map<UID, PxRigidActor*> rigidMap;

	void Init();
	void Tick(float deltaTime);
	void Cleanup();
	void Reset();

	void CreateRigidDynamicPhysicsActor(MeshComponent* mesh);
	void CreateRigidStaticPhysicsActor(MeshComponent* mesh);
	void ActorToPhysxTransform(const Transform& actorTransform, PxTransform& pxTransform);
	void PhysxToActorTransform(Transform& actorTransform, const PxTransform& pxTransform);
	void GetTransformFromPhysicsActor(MeshComponent* mesh);
};

extern PhysicsSystem physicsSystem;
