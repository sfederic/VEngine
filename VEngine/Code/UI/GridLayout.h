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

	void SetLayouts(const int numRows, const int numColumns, const Layout& gridBounds, const float spacing = 0.f)
	{
		Layout columnLayout = gridBounds;
		columnLayout.PushToTop(spacing);
		columnLayout.PushToLeft(spacing);

		const float rowIncrement = (gridBounds.rect.bottom - gridBounds.rect.top) / numRows;
		columnLayout.rect.bottom = columnLayout.rect.top + rowIncrement;

		for (int rowIndex = 0; rowIndex < numRows; rowIndex++)
		{
			AddRow();

			const float columnIncrement = (gridBounds.rect.right - gridBounds.rect.left) / numColumns;
			columnLayout.rect.right = columnLayout.rect.left + columnIncrement;

			for (int columnIndex = 0; columnIndex < numColumns; columnIndex++)
			{
				AddColumn(rowIndex, columnLayout);
				columnLayout.AddHorizontalSpace(columnIncrement + spacing);
			}

			//Reset left back to original extent
			columnLayout.rect.left = gridBounds.rect.left;

			columnLayout.AddVerticalSpace(rowIncrement + spacing);
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
