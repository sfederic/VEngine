#include "vpch.h"
#include "BattleCard.h"
#include "BattleCardSystem.h"
#include "Actors/Game/Player.h"

struct DamageTrapCard : BattleCard
{
	DamageTrapCard() : BattleCard(L"DamageTrapCard", L"Inflicts damage.", "UI/trap_icon.jpg")
	{
		BattleCardSystem::Get().AddCard(this);
	}

	virtual void Activate() override
	{
		__super::Activate();
		Player::system.GetFirstActor()->PlaceTrap(this);
	}
}damageTrapCard;

