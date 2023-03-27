#include "vpch.h"
#include "BattleCardSystem.h"
#include "BattleCard.h"
#include "Core/VMath.h"

void BattleCardSystem::AddCard(BattleCard* card)
{
	assert(battleCardMap.find(card->name) == battleCardMap.end());
	battleCardMap.emplace(card->name, card);
}

BattleCard* BattleCardSystem::DrawCardAtRandom()
{
	auto battleCardMapIt = battleCardMap.begin();
	int rand = VMath::RandomRangeInt(0, battleCardMap.size() - 1);
	std::advance(battleCardMapIt, rand);
	return battleCardMapIt->second;
}

std::vector<BattleCard*> BattleCardSystem::GetAllCards(std::wstring filter)
{
	std::vector<BattleCard*> cards;
	cards.reserve(battleCardMap.size());
	for (auto& [key, value] : battleCardMap)
	{
		if (key.find(filter) != std::wstring::npos)
		{
			cards.emplace_back(value);
		}
	}
	return cards;
}
