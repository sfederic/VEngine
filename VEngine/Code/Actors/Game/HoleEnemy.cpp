#include "vpch.h"
#include "HoleEnemy.h"
#include "Components/MeshComponent.h"

HoleEnemy::HoleEnemy()
{
}

void HoleEnemy::Create()
{
	core = CreateComponent(MeshComponent("sphere.vmesh", "test.png"), "CoreMesh");
	core->SetLocalScale(0.5f, 0.5f, 0.5f);
	rootComponent = core;

	hull = CreateComponent(MeshComponent("hole_enemy.vmesh", "test.png"), "HullMesh");
	rootComponent->AddChild(hull);
}
