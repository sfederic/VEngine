#include "vpch.h"
#include "PickupWidget.h"
#include "Gameplay/GameInstance.h"

void PickupWidget::Draw(float deltaTime)
{
	if (!GameInstance::GetHeldPlayerItem().empty())
	{
		auto layout = PercentAlignLayout(0.8f, 0.8f, 0.9f, 0.9f);
		Image(heldItemIconFilename, layout);
	}
}
