#pragma once

#define PX_PHYSX_STATIC_LIB

#include <unordered_map>
#include <foundation/PxTransform.h>
#include <PxRigidActor.h>
#include <DirectXMath.h>
#include "UID.h"

using namespace physx;
using namespace DirectX;

struct MeshComponent;
struct Transform;
struct Actor;

enum class PhysicsType
{
	Static,
	Dynamic
};


//Interface to PhysX systems
//Ref: https://gameworksdocs.nvidia.com/PhysX/4.1/documentation/physxguide/Index.html
struct PhysicsSystem
{
	//Maps meshcomponent UIDs to rigid actors
	std::unordered_map<UID, PxRigidActor*> rigidActorMap;

	void Init();
	void Start();
	void Tick(float deltaTime);
	void Cleanup();
	void Reset();

	void CreatePhysicsActor(MeshComponent* mesh, PhysicsType type, Actor* actor);
	void ActorToPhysxTransform(const Transform& actorTransform, PxTransform& pxTransform);
	void PhysxToActorTransform(Transform& actorTransform, const PxTransform& pxTransform);
	void GetTransformFromPhysicsActor(MeshComponent* mesh);

private:
	void NormaliseExtents(float& x, float& y, float& z);
};

extern PhysicsSystem physicsSystem;

namespace Physics
{
	struct RaycastHit
	{
		Actor* hitActor = nullptr;

		XMFLOAT3 normal;
		XMFLOAT3 posiiton;
		XMFLOAT2 uv;

		float distance = 0.f;
	};

	bool Raycast(XMFLOAT3 origin, XMFLOAT3 dir, float range, RaycastHit& hit);
}
