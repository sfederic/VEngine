#include "vpch.h"
#include "BattleCard.h"

BattleCard::BattleCard(std::wstring name_, std::wstring desc_, std::string imageFilename_)
{
	name = name_;
	desc = desc_;
	imageFilename = imageFilename_;
}
