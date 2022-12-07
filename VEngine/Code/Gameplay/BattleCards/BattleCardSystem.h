#pragma once

#include <map>

struct BattleCard;

class BattleCardSystem
{
public:
	static BattleCardSystem& Get()
	{
		static BattleCardSystem battleCardSystem;
		return battleCardSystem;
	}

	void AddCard(BattleCard* card);
	BattleCard* DrawCardAtRandom();

private:
	std::map<std::wstring, BattleCard*> battleCardMap;
};
