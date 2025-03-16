
#include "Burnable.h"
#include "Gameplay/GameUtils.h"
#include "Core/Timer.h"
import Core.VMath;
#include "Core/Core.h"
#include "Player.h"

Properties Burnable::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}

void Burnable::Burn()
{
	if (onFire)
	{
		burnSpriteTimer += Core::GetDeltaTime();
		if (burnSpriteTimer > 0.1f)
		{
			burnSpriteTimer = 0.f;
			const auto randomDirection = GetPositionV() + VMath::RandomUnitDirectionVector();
			GameUtils::SpawnSpriteSheet("Sprites/fire_sheet.png", randomDirection, false, 4, 4);
		}

		return;
	}

	Timer::SetTimer(2.f, std::bind(&Burnable::BurnDelay, this));
	onFire = true;
}

void Burnable::BurnDelay()
{
	Player::system.GetOnlyActor()->ResetLinkedGridActorIfThis(this);
	Remove();
}
