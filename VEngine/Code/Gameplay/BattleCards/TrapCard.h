#pragma once

#include "BattleCard.h"
#include "Gameplay/GridNode.h"

struct TrapCard : BattleCard
{
	TrapCard(std::wstring name_, std::wstring desc_, std::string imageFilename_) :
		BattleCard(name_, desc_, imageFilename_) {}

	virtual void Set();
	virtual void ActivateTrap() = 0;

	GridNode* connectedNode = nullptr;
};
