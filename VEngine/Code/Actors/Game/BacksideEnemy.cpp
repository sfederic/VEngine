#include "vpch.h"
#include "BacksideEnemy.h"
#include "Components/MeshComponent.h"
#include "Actors/Game/Player.h"
#include "Core/VMath.h"

void BacksideEnemy::Create()
{
	__super::Create();

	auto mesh = CreateComponent("Mesh", MeshComponent("turret.vmesh", "test.png"));
	rootComponent->AddChild(mesh);
}

bool BacksideEnemy::CanBeHit(AttackTypes attackType)
{
	auto player = Player::system.GetFirstActor();
	auto playerForward = player->GetForwardVectorV();
	auto forward = GetForwardVectorV();

	VMath::RoundVector(playerForward);
	VMath::RoundVector(forward);

	if (XMVector4Equal(playerForward, forward))
	{
		return true;
	}

	return false;
}
