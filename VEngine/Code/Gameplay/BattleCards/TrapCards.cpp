#include "vpch.h"
#include "BattleCard.h"
#include "BattleCardSystem.h"

class TestTrapNodeCard : BattleCard
{
public:
	TestTrapNodeCard() : BattleCard("TestTrapNodeCard", "test.png")
	{
		BattleCardSystem::Get().AddCard<TestTrapNodeCard>();
	}
}testTrapNodeCard;
