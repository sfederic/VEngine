#pragma once

#include <vector>
#include "Layout.h"

class GridLayout
{
public:
	Layout GetLayoutAt(int x, int y)
	{
		return rows.at(x).columns.at(y);
	}

	void SetLayouts(const int numRows, const int numColumns, const Layout& gridBounds)
	{
		Layout columnLayout = gridBounds;
		columnLayout.PushToTop();
		columnLayout.PushToLeft();

		for (int rowIndex = 1; rowIndex < numRows + 1; rowIndex++)
		{
			AddRow();

			const float rowIncrement = ((gridBounds.rect.bottom - gridBounds.rect.top) / numRows);
			columnLayout.rect.bottom = columnLayout.rect.top + rowIncrement;

			const float columnIncrement = ((gridBounds.rect.right - gridBounds.rect.left) / numColumns);
			columnLayout.rect.right = columnLayout.rect.left + columnIncrement;

			for (int columnIndex = 1; columnIndex < numColumns + 1; columnIndex++)
			{
				AddColumn(rowIndex - 1, columnLayout);
				columnLayout.AddHorizontalSpace(columnIncrement);
			}

			columnLayout.rect.left = gridBounds.rect.left;
			columnLayout.rect.top += rowIncrement;
		}
	}

private:
	struct GridLayoutRow
	{
		std::vector<Layout> columns;

		void Add(Layout& layout)
		{
			columns.emplace_back(layout);
		}
	};

	void AddRow()
	{
		rows.emplace_back(GridLayoutRow());
	}

	void AddColumn(int rowIndex, Layout& layout)
	{
		rows.at(rowIndex).Add(layout);
	}
	std::vector<GridLayoutRow> rows;
};
