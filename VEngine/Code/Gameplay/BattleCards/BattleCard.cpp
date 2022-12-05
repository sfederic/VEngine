#include "vpch.h"
#include "BattleCard.h"
#include "BattleCardSystem.h"

BattleCard::BattleCard(std::string name_, std::string imageFilename_)
{
	name = name_;
	imageFilename = imageFilename_;

	BattleCardSystem::Get()
}
