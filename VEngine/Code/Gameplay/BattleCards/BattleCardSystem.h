#pragma once

#include <map>
#include <memory>
#include "Gameplay/BattleCards/BattleCard.h"

class BattleCardSystem
{
public:
	static BattleCardSystem& Get()
	{
		static BattleCardSystem battleCardSystem;
		return battleCardSystem;
	}

	template <typename T>
	void AddCard()
	{
		auto card = std::make_unique<T>();
		battleCardMap.emplace(card->name, std::move(card));
	}

private:
	std::map<std::string, std::unique_ptr<BattleCard>> battleCardMap;
};
