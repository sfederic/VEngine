#pragma once

#include <string>

struct GridNode;

//Holds data for 'card's the player can use during combat.
struct BattleCard
{
	BattleCard() {}
	BattleCard(std::wstring name_, std::wstring desc_, std::string imageFilename_);

	virtual void Activate();

	std::wstring name;
	std::wstring desc;
	std::string imageFilename;
};
