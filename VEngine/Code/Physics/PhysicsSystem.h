#pragma once

#define PX_PHYSX_STATIC_LIB

#include <unordered_map>
#include <foundation/PxTransform.h>
#include <PxRigidActor.h>
#include "UID.h"

using namespace physx;

struct MeshComponent;
struct Transform;

enum class PhysicsType
{
	Static,
	Dynamic
};

//Interface to PhysX systems
struct PhysicsSystem
{
	//Maps meshcomponent UIDs to rigid actors
	std::unordered_map<UID, PxRigidActor*> rigidActorMap;

	void Init();
	void Tick(float deltaTime);
	void Cleanup();
	void Reset();

	void CreatePhysicsActor(MeshComponent* mesh, PhysicsType type);
	void ActorToPhysxTransform(const Transform& actorTransform, PxTransform& pxTransform);
	void PhysxToActorTransform(Transform& actorTransform, const PxTransform& pxTransform);
	void GetTransformFromPhysicsActor(MeshComponent* mesh);

private:
	void NormaliseExtents(float& x, float& y, float& z);
};

extern PhysicsSystem physicsSystem;
