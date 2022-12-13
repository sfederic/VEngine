#pragma once

#include <map>
#include <vector>

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
	std::vector<BattleCard*> GetAllCards(std::wstring filter);

private:
	std::map<std::wstring, BattleCard*> battleCardMap;
};
