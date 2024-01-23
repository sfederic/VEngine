#include "vpch.h"
#include "JournalWidget.h"
#include "Gameplay/JournalSystem.h"
#include "UI/GridLayout.h"

#include "Core/VMath.h"
#include "Core/VString.h"

void JournalWidget::Draw(float deltaTime)
{
	auto gridOverallLayout = PercentAlignLayout(0.2f, 0.2f, 0.8f, 0.8f);
	const int numRows = 4;
	const int numColumns = 4;
	GridLayout gridLayout;
	gridLayout.SetLayouts(numRows, numColumns, gridOverallLayout);

	for (int i = 0; i < numRows; i++)
	{
		for (int j = 0; j < numColumns; j++)
		{
			auto layout = gridLayout.GetLayoutAt(i, j);
			if (Button(VString::format("%d %d", i, j), layout))
			{
				Log("%d %d clicked.", i, j);
			}
		}
	}
}
