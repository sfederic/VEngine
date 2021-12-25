#include "PhysicsSystem.h"

using namespace physx;

PxDefaultAllocator allocator;
PxDefaultErrorCallback errorCallback;

PxFoundation* foundation = nullptr;
PxPhysics* physics = nullptr;

PxDefaultCpuDispatcher* dispatcher = nullptr;
PxScene* scene = nullptr;

PxMaterial* material = nullptr;

PxPvd* pvd = nullptr;

PxReal stackZ = 10.0f;

void PhysicsSystem::Init()
{
	foundation = PxCreateFoundation(PX_PHYSICS_VERSION, allocator, errorCallback);
}
