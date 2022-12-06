#include "vpch.h"
#include "BattleCardSystem.h"
#include "BattleCard.h"

void BattleCardSystem::AddCard(BattleCard* card)
{
	if (battleCardMap == nullptr)
	{
		battleCardMap = new std::map<std::wstring, BattleCard*>();
	}

	battleCardMap->emplace(card->name, card);
}

BattleCard* BattleCardSystem::DrawCardAtRandom()
{
	auto battleCardMapIt = battleCardMap->begin();
	std::advance(battleCardMapIt, std::rand() % battleCardMap->size());
	return battleCardMapIt->second;
}
