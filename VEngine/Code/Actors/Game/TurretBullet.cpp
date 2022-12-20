#include "vpch.h"
#include "TurretBullet.h"
#include "Components/MeshComponent.h"

TurretBullet::TurretBullet()
{
	auto mesh = CreateComponent(MeshComponent("sphere.fbx", "test.png"), "Mesh");
	mesh->SetScale(0.5f, 0.5f, 0.5f);
	rootComponent = mesh;
}

void TurretBullet::Tick(float deltaTime)
{
	auto pos = GetPositionV();
	pos += movementDirection * moveSpeed * deltaTime;
	SetPosition(pos);
}
