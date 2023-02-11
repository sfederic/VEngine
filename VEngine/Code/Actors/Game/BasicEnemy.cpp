#include "vpch.h"
#include "BasicEnemy.h"
#include "Components/MeshComponent.h"

BasicEnemy::BasicEnemy()
{
	rootComponent = CreateComponent("Mesh", MeshComponent("ico_sphere.vmesh", "test.png"));
}

void BasicEnemy::Tick(float deltaTime)
{
	constexpr float rotateSpeed = 55.f;
	AddRotation(GetUpVectorV(), rotateSpeed * deltaTime);
}
