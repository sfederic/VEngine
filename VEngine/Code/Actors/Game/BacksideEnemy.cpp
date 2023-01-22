#include "vpch.h"
#include "BacksideEnemy.h"
#include "Components/MeshComponent.h"
#include "Actors/Game/Player.h"
#include "VMath.h"

void BacksideEnemy::Create()
{
	auto mesh = CreateComponent(MeshComponent("turret.vmesh", "test.png"), "Mesh");
	rootComponent->AddChild(mesh);
}

bool BacksideEnemy::CanBeHit()
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
