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

using namespace physx;
using namespace DirectX;

class MeshComponent;
class DestructibleMeshComponent;
class CharacterControllerComponent;
struct Transform;
class Actor;

enum class PhysicsType
{
	Static,
	Dynamic
};

//@Todo: Switching the project config to Release works bu the entire PhysX debug dlls and libs have to be replaced 
//with the release specific ones. Try find a way to change this with a build file process so that switching to 
//Release doesn't need Ctrl+V'ing everything.

//@Todo: if non static MeshComponents are inside a skybox for example, they'll explode out because that
//skybox hasn't disabled its BoundingBox and/or PhysicsComponent. make a flag that turns it off.

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
	std::vector<PxRigidActor*> GetRigidActors();
	void CreatePhysicsActor(MeshComponent* mesh, PhysicsType type);
	void CreatePhysicsForDestructibleMesh(DestructibleMeshComponent* mesh, Actor* actor);
	void CreateCharacterController(CharacterControllerComponent* characterControllerComponent);
	void CreateConvexPhysicsMesh(MeshComponent* mesh, Actor* actor);

	//"Collision mesh" meaning a rough bounds mesh made in DCC tool. Pass in the mesh you want to 
	//attach the physics actor to, with the filename being the collision mesh.
	void CreateConvexPhysicsMeshFromCollisionMesh(MeshComponent* mesh, Actor* actor, const std::string filename);

	void ActorToPhysxTransform(const Transform& actorTransform, PxTransform& pxTransform);
	void PhysxToActorTransform(Transform& actorTransform, const PxTransform& pxTransform);
	void GetTransformFromPhysicsActor(MeshComponent* mesh);

	std::unordered_map<UID, std::unique_ptr<MeshComponent>>& GetAllPhysicsMeshes();
};

namespace Physics
{
	//For now, use this raycast if you only want to test against bounding boxes with no mesh triangle intersects.
	//@Todo: bring in triangle intersects and flesh out the PhysX raycast. Get rid of older Raycast.
	bool Raycast(XMFLOAT3 origin, XMFLOAT3 dir, float range, HitResult& hitResult);
	bool BoxCast(XMFLOAT3 extents, XMFLOAT3 origin, XMFLOAT3 direction, float distance, HitResult& hitResult);

	PxVec3 Float3ToPxVec3(XMFLOAT3 float3);
	XMFLOAT3 PxVec3ToFloat3(PxVec3 pxVec3);
}
