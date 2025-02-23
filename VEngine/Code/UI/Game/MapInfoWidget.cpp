#include "vpch.h"
#include "MapInfoWidget.h"
#include "Gameplay/WorldFunctions.h"
#include "Core/World.h"

void MapInfoWidget::Draw(float deltaTime)
{
	__super::Draw(deltaTime);

	const auto layout = PercentAlignLayout(0.7f, 0.1f, 0.9f, 0.3f);
	FillRect(layout);
	const auto worldDisplayName = WorldFunctions::GetWorldDisplayName(World::worldFilename);
	Text(worldDisplayName, layout);
}
