#include "vpch.h"
#include "OrientationEnemy.h"
#include "Core/VMath.h"
#include "Components/MeshComponent.h"
#include "Actors/Game/Player.h"

OrientationEnemy::OrientationEnemy()
{
	auto mesh = CreateComponent("Mesh", MeshComponent("orientation_enemy.vmesh", "orientation_enemy.jpg"));
	rootComponent->AddChild(mesh);
}

void OrientationEnemy::Tick(float deltaTime)
{
	AddRotation(GetUpVectorV(), 15.f * deltaTime);
}

bool OrientationEnemy::CanBeHit(AttackTypes attackType)
{
	if (attackType != AttackTypes::Melee) return false;

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
