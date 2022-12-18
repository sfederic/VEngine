#include "vpch.h"
#include "Enemy.h"
#include "Components/MeshComponent.h"

Enemy::Enemy()
{
	mesh = CreateComponent(MeshComponent("cube.fbx", "test.png"), "Mesh");
	mesh->SetScale(0.8f, 0.8f, 0.8f);
	rootComponent = mesh;
}

void Enemy::Tick(float deltaTime)
{
	auto rot = GetRotationV();
	rot = XMQuaternionMultiply(XMQuaternionRotationAxis(GetUpVectorV(), deltaTime), rot);
	SetRotation(rot);
}

Properties Enemy::GetProps()
{
	auto props = __super::GetProps();
	props.title = "Enemy";
	return props;
}
