#include "vpch.h"
#include "Enemy.h"
#include "Components/MeshComponent.h"

Enemy::Enemy()
{
	rootComponent = CreateComponent(MeshComponent("cube.fbx", "test.png"), "Mesh");
}

void Enemy::Start()
{
}

void Enemy::Tick(float deltaTime)
{
}
