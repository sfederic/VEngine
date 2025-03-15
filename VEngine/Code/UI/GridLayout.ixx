export module UI.GridLayout;

import UI.Layout;
import <vector>;

//Note that the vernacular with this class can be a bit funny with how its referencing columns, that being
//that a 'column' is more like a single 'entry' along a row rather than an entire literal column of data.
export class GridLayout
{
public:
	Layout GetLayoutAt(int rowIndex, int columnIndex)
	{
		return rows.at(rowIndex).columns.at(columnIndex);
	}

	std::vector<Layout> GetAllLayouts()
	{
		std::vector<Layout> layouts;
		for (GridLayoutRow& row : rows)
		{
			for (Layout layout : row.columns)
			{
				layouts.emplace_back(layout);
			}
		}
		return layouts;
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
