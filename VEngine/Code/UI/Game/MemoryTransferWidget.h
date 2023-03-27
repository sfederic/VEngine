#pragma once
#include "../Widget.h"
#include <map>

struct Memory;

//Widget to select which Memories to transfer over to new game (from game overs or whatever).
struct MemoryTransferWidget : Widget
{
	std::map<std::string, Memory*> selectedMemoriesToTransfer;
	
	virtual void Draw(float deltaTime) override;
};
