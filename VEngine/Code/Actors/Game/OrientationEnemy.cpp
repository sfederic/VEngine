#include "vpch.h"
#include "OrientationEnemy.h"
#include "VMath.h"
#include "Components/MeshComponent.h"
#include "Actors/Game/Player.h"

void OrientationEnemy::Create()
{
	auto mesh = CreateComponent(MeshComponent("cube.vmesh", "up_arrow.png"), "Mesh");
	rootComponent->AddChild(mesh);
}

bool OrientationEnemy::CanBeHit()
{
	auto player = Player::system.GetFirstActor();
	auto playerUp = player->GetUpVectorV();
	auto enemyUp = GetUpVectorV();

	VMath::RoundVector(playerUp);
	VMath::RoundVector(enemyUp);

	if (XMVector4Equal(playerUp, enemyUp))
	{
		return true;
	}

	return false;
}
