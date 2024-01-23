#include "vpch.h"
#include "JournalWidget.h"
#include "Gameplay/JournalSystem.h"
#include "UI/GridLayout.h"

#include "Core/VMath.h"
#include "Core/VString.h"

void JournalWidget::Draw(float deltaTime)
{
	const auto gridOverallLayout = PercentAlignLayout(0.1f, 0.1f, 0.9f, 0.9f);
	const int numRows = 3;
	const int numColumns = 3;
	GridLayout gridLayout;
	gridLayout.SetLayouts(numRows, numColumns, gridOverallLayout, 20.f);

	const std::vector<Layout> gridLayouts = gridLayout.GetAllLayouts();
	int gridLayoutIndex = 0;

	for (const auto& [title, entry] : JournalSystem::Get().GetJournalEntries())
	{
		const auto& layout = gridLayouts.at(gridLayoutIndex);
		if (Button(title, layout))
		{
			Log("%s clicked.", title.c_str());
		}
		gridLayoutIndex++;
	}
}
