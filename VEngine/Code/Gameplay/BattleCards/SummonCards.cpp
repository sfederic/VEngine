#include "vpch.h"
#include "BattleCard.h"
#include "BattleCardSystem.h"
#include "Actors/Game/Player.h"

#include "Actors/Game/AllyUnits/SurroundAttackUnit.h"

struct TestSummonCard : BattleCard
{
	TestSummonCard() : BattleCard(L"TestSummonCard", L"Summons a test monster", "UI/monster_icon.jpg")
	{
		cost = 2;
		BattleCardSystem::Get().AddCard(this);
	}

	virtual void Activate() override
	{
		__super::Activate();
		Player::system.GetFirstActor()->SummonAllyUnit<SurroundAttackUnit>();
	}
}testSummonCard;
