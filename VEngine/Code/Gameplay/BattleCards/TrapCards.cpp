#include "vpch.h"
#include "BattleCard.h"
#include "BattleCardSystem.h"
#include "Actors/Game/Player.h"

struct TestTrapNodeCard : BattleCard
{
	TestTrapNodeCard() : BattleCard(L"TestTrapNodeCard", L"Testing", "test.png")
	{
		BattleCardSystem::Get().AddCard(this);
	}

	virtual void Activate() override
	{
		Player::system.GetFirstActor()->PlaceTrap<TestTrapNodeCard>();
	}
}testTrapNodeCard;
