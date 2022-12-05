#pragma once

#include <string>

//Holds data for 'card's the player can use during combat.
class BattleCard
{
public:
	BattleCard(std::string name_, std::string imageFilename_);

	std::string name;
	std::string imageFilename;
};
