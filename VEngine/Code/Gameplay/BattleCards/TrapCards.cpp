#include "vpch.h"
#include "BattleCard.h"
#include "BattleCardSystem.h"

struct TestTrapNodeCard : BattleCard
{
	TestTrapNodeCard() : BattleCard("TestTrapNodeCard", "test.png")
	{
		BattleCardSystem::Get().AddCard(this);
	}
}testTrapNodeCard;
