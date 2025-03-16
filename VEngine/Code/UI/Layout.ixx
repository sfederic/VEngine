#include <d2d1.h>

export module UI.Layout;

export struct Layout
{
	D2D1_RECT_F rect = {};

	Layout() {}
	Layout(D2D1_RECT_F rect_) : rect(rect_) {}

	void AddVerticalSpace(float spacing)
	{
		rect.top += spacing;
		rect.bottom += spacing;
	}

	void AddHorizontalSpace(float spacing)
	{
		rect.left += spacing;
		rect.right += spacing;
	}

	void PushDown(float spacing)
	{
		rect.top += spacing;
	}

	void PushRight(float spacing)
	{
		rect.right += spacing;
	}

	void PushToLeft(float spacing = 0.f)
	{
		rect.right = rect.left + spacing;
	}

	//PushToTop is good for spacing out lists so that the widget item is compact, not reaching the entire layout.
	void PushToTop(float spacing = 0.f)
	{
		rect.bottom = rect.top + spacing;
	}

	bool IsPosInLayout(int x, int y) const
	{
		if (x > rect.left && x < rect.right)
		{
			if (y > rect.top && y < rect.bottom)
			{
				return true;
			}
		}
		return false;
	}
};
