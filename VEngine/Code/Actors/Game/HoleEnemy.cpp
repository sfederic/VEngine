#include "vpch.h"
#include "HoleEnemy.h"
#include "Components/MeshComponent.h"
#include "Gameplay/GameplayTags.h"

HoleEnemy::HoleEnemy()
{
}

void HoleEnemy::Create()
{
	hull = CreateComponent(MeshComponent("hole_enemy.vmesh", "test.png"), "HullMesh");
	hull->SetRastState(RastStates::noBackCull);
	hull->AddTag(GameplayTags::InvincibleMeshPiece);
	rootComponent->AddChild(hull);

	core = CreateComponent(MeshComponent("sphere.vmesh", "test.png"), "CoreMesh");
	core->SetLocalScale(0.25f);
	core->SetUseTexture(false);
	core->SetAmbientColour(XMFLOAT4(0.9f, 0.2f, 0.1f, 1.f));
	core->AddTag(GameplayTags::EnemyMeshPiece);
	rootComponent->AddChild(core);
}
