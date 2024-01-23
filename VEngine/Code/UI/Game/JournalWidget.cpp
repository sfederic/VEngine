#include "vpch.h"
#include "JournalWidget.h"
#include "Gameplay/JournalSystem.h"
#include "UI/GridLayout.h"
#include "Core/Input.h"
#include "Core/Core.h"

struct WidgetEntry
{
	JournalEntry entry;
	Layout layout;
};

void JournalWidget::Draw(float deltaTime)
{
	DrawJournalEntriesToGridLayout();
}

void JournalWidget::DrawJournalEntriesToGridLayout()
{
	const auto gridOverallLayout = PercentAlignLayout(0.1f, 0.1f, 0.9f, 0.9f);
	const int numRows = 3;
	const int numColumns = 3;
	GridLayout gridLayout;
	gridLayout.SetLayouts(numRows, numColumns, gridOverallLayout, 20.f);

	const std::vector<Layout> gridLayouts = gridLayout.GetAllLayouts();
	int gridLayoutIndex = 0;

	std::vector<WidgetEntry> widgetEntries;

	for (auto& [title, entry] : JournalSystem::Get().GetJournalEntries())
	{
		const auto& layout = gridLayouts.at(gridLayoutIndex);
		if (Button(title, layout))
		{
			selectedJournalEntry = &entry;
		}
		gridLayoutIndex++;
	}

	if (selectedJournalEntry != nullptr)
	{
		auto entryLayout = PercentAlignLayout(0.3f, 0.3f, 0.7f, 0.7f);
		FillRect(entryLayout);
		Text(selectedJournalEntry->title, entryLayout);
		entryLayout.AddVerticalSpace(30.f);
		Text(selectedJournalEntry->text, entryLayout);
		entryLayout.AddVerticalSpace(30.f);
	}

	//Close widget from here since player logic will pause game world.
	if (Input::GetKeyUp(Keys::J))
	{
		Core::UnPauseGameWorld();
		this->RemoveFromViewport();
	}
}
