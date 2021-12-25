#pragma once

#define PX_PHYSX_STATIC_LIB

struct PhysicsSystem
{
	void Init();
	void Tick(float deltaTime);
	void Cleanup();
};

extern PhysicsSystem physicsSystem;
