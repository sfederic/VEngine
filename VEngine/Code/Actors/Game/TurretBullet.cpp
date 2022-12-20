#include "vpch.h"
#include "TurretBullet.h"
#include "Components/MeshComponent.h"
#include "Components/BoxTriggerComponent.h"
#include "Actors/Game/TurretEnemy.h"
#include "Physics/Raycast.h"

TurretBullet::TurretBullet()
{
	auto mesh = CreateComponent(MeshComponent("small_ico_sphere.fbx", "test.png"), "Mesh");
	rootComponent = mesh;

	boxTrigger = CreateComponent(BoxTriggerComponent(), "BoxTrigger");
	boxTrigger->boundingBox.Extents = XMFLOAT3(0.2f, 0.2f, 0.2f);
	rootComponent->AddChild(boxTrigger);
}

void TurretBullet::Tick(float deltaTime)
{
	auto pos = GetPositionV();
	pos += movementDirection * moveSpeed * deltaTime;
	SetPosition(pos);

	Ray ray(this);
	auto turretEnemies = TurretEnemy::system.GetActorsAsBaseClass();
	ray.AddActorsToIgnore(turretEnemies);
	if (SimpleBoxCast(GetPosition(), boxTrigger->boundingBox.Extents, ray))
	{
		Destroy();
	}
}
