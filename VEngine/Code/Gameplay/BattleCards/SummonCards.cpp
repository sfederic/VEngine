#include "vpch.h"
#include "BattleCard.h"
#include "BattleCardSystem.h"
#include "Actors/Game/Player.h"

struct TestSummonCard : BattleCard
{
	TestSummonCard() : BattleCard(L"TestSummonCard", L"Summons a test monster", "monster_icon.png")
	{
		cost = 2;
		BattleCardSystem::Get().AddCard(this);
	}

	virtual void Activate() override
	{
		__super::Activate();
		Player::system.GetFirstActor()->PlaceTrap(this);
	}
}testSummonCard;
