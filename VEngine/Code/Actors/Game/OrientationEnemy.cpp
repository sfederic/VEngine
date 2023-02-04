#include "vpch.h"
#include "OrientationEnemy.h"
#include "Core/VMath.h"
#include "Components/MeshComponent.h"
#include "Actors/Game/Player.h"

void OrientationEnemy::Create()
{
	auto mesh = CreateComponent(MeshComponent("orientation_enemy.vmesh", "test.png"), "Mesh");
	rootComponent->AddChild(mesh);
}

bool OrientationEnemy::CanBeHit(AttackTypes attackType)
{
	if (attackType != AttackTypes::Melee) return;

	auto player = Player::system.GetFirstActor();
	auto playerUp = player->GetUpVectorV();
	auto enemyUp = GetUpVectorV();

	VMath::RoundVector(playerUp);
	VMath::RoundVector(enemyUp);

	if (XMVector4Equal(playerUp, enemyUp) || XMVector4Equal(-playerUp, enemyUp))
	{
		return true;
	}

	return false;
}
