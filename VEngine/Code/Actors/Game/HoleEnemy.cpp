#include "vpch.h"
#include "HoleEnemy.h"
#include "Components/MeshComponent.h"
#include "Gameplay/GameplayTags.h"
#include "Render/RastStates.h"

void HoleEnemy::Create()
{
	__super::Create();

	hull = CreateComponent("HullMesh", MeshComponent("hole_enemy.vmesh", "test.png"));
	hull->SetRastState(RastStates::noBackCull);
	hull->AddTag(GameplayTags::InvincibleMeshPiece);
	rootComponent->AddChild(hull);

	core = CreateComponent("CoreMesh", MeshComponent("sphere.vmesh", "test.png"));
	core->SetLocalScale(0.25f);
	core->SetUseTexture(false);
	core->SetAmbientColour(XMFLOAT4(0.9f, 0.2f, 0.1f, 1.f));
	core->AddTag(GameplayTags::EnemyMeshPiece);
	rootComponent->AddChild(core);
}
