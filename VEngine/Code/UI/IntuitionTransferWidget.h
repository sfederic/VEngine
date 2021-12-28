#pragma once
#include "Widget.h"
#include <map>

struct Intuition;

//Widget to select which Intuitions to transfer over to new game (from game overs or whatever).
struct IntuitionTransferWidget : Widget
{
	std::map<std::string, Intuition*> selectedIntuitionsToTransfer;

	virtual void Tick(float deltaTime) override;
};
