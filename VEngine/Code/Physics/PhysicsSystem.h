#pragma once

#define PX_PHYSX_STATIC_LIB

#include <unordered_map>
#include <foundation/PxTransform.h>
#include <PxRigidActor.h>
#include <memory>
#include <string>
#include <DirectXMath.h>
#include "Core/UID.h"
#include "Physics/HitResult.h"
#include "Physics/PhysicsActorShape.h"
#include "Physics/PhysicsType.h"

using namespace physx;
using namespace DirectX;

class MeshComponent;
class DestructibleMeshComponent;
class CharacterControllerComponent;
struct Transform;
class Actor;

//@Todo: Switching the project config to Release works bu the entire PhysX debug dlls and libs have to be replaced 
//with the release specific ones. Try find a way to change this with a build file process so that switching to 
//Release doesn't need Ctrl+V'ing everything.

//Interface to PhysX systems
//Ref: https://gameworksdocs.nvidia.com/PhysX/4.1/documentation/physxguide/Index.html
namespace PhysicsSystem
{
	void Init();
	void Start();
	void Tick(float deltaTime);
	void Cleanup();
	void Reset();
	void ReleasePhysicsActor(MeshComponent* mesh);
	void CreatePhysicsActor(MeshComponent* mesh, const PhysicsActorShape physicsActorShape);
	void CreatePhysicsForDestructibleMesh(DestructibleMeshComponent* mesh);
	void CreateCharacterController(CharacterControllerComponent* characterControllerComponent);
	void CreateConvexPhysicsMesh(MeshComponent* mesh);

	//"Collision mesh" meaning a rough bounds mesh made in DCC tool. Pass in the mesh you want to 
	//attach the physics actor to, with the filename being the collision mesh.
	void CreateConvexPhysicsMeshFromCollisionMesh(MeshComponent* mesh, Actor* actor, const std::string filename);

	void ActorToPhysxTransform(const Transform& actorTransform, PxTransform& pxTransform);
	void PhysxToActorTransform(Transform& actorTransform, const PxTransform& pxTransform);
	void GetTransformFromPhysicsActor(MeshComponent* mesh);
	void SetTransformForPhysicsActor(MeshComponent* mesh);

	void AddForceToMesh(MeshComponent* mesh, XMVECTOR forceDirection);

	std::unordered_map<UID, std::unique_ptr<MeshComponent>>& GetAllPhysicsMeshes();
};

namespace PhysicsPhysx
{
	bool Raycast(XMVECTOR origin, XMVECTOR direction, float range, HitResult& hitResult);
	bool BoxCast(XMFLOAT3 extents, XMFLOAT3 origin, XMFLOAT3 direction, float distance, HitResult& hitResult);

	PxVec3 XMVectorToPxVec3(XMVECTOR xmVector);
	PxQuat XMVectorToPxQuat(XMVECTOR xmVector);
	PxVec3 Float3ToPxVec3(XMFLOAT3 float3);
	XMFLOAT3 PxVec3ToFloat3(PxVec3 pxVec3);
}
