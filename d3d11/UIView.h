#pragma once

#include <dwrite_1.h>
#include <vector>
#include <string>

//base struct for UI elements to inherit from
struct UIView
{
	D2D1_RECT_F viewRectBack;
	D2D1_RECT_F viewRect;
	D2D1_RECT_F viewRectOffset;

	int idCounter = 0; //Keeps track of all ids to link to above activeElements

	void Init(D2D1_RECT_F viewRect_, const wchar_t* title);

	virtual void Tick() {}

	void Text(const wchar_t* string);
	bool Button(const wchar_t* string);

	template <typename T>
	void Edit(T& editVal, std::wstring& editString);

	void NewLine();
	void NewLine(int numOfNewlines);
	void IncrementViewRect();
};

//TODO: See if throwing this into another .h file will lower compile times
struct UIViewActor : public UIView
{
	virtual void Tick();

	std::wstring posString;
};

//TODO: Figrue out whether this sort of template work would just be better of with overloaded functions. 
//Header file decl. like this might hurt compiles later. eg. Editfloat, EditInt, EditString
template<typename T>
inline void UIView::Edit(T& editVal, std::wstring& editString)
{
	if (gUISystem.activeUIViewElementIndex == idCounter)
	{
		gUISystem.bEditUIActive = true;

		gUISystem.d2dRenderTarget->DrawRectangle(viewRect, gUISystem.brushTextBlack);

		//HANDLE TEXT INPUT

		if (inputSystem.GetAsyncKey(VK_CONTROL)) //Delete all characters
		{
			if (inputSystem.GetKeyDownState(VK_BACK))
			{
				editString.clear();
			}
		}
		else if (inputSystem.GetKeyDownState(VK_BACK)) //Backspace
		{
			if (editString.size() > 0)
			{
				editString.pop_back();
			}
		}
		else if (inputSystem.GetKeyDownState(VK_RETURN)) //Enter value
		{
			editVal = (T)_wtof(editString.c_str());
			editString = std::to_wstring(editVal);

			gUISystem.activeUIViewElementIndex = -1;
			gUISystem.bEditUIActive = false;
		}
		else if (inputSystem.GetKeyDownState(VK_OEM_PERIOD)) //(./>) key
		{
			editString.push_back('.');
		}
		else if (inputSystem.GetAnyKeyDown()) //Careful with the else-if here on GetAnyKey
		{
			const int maxEditStringSize = 64;
			if (editString.size() < maxEditStringSize)
			{
				wchar_t key = inputSystem.currentDownKey;
				editString.push_back(key);
			}
		}

		gUISystem.d2dRenderTarget->DrawTextA(editString.c_str(), editString.size(), gUISystem.textFormat, viewRect, gUISystem.brushText);
	}
	else
	{
		editString = std::to_wstring(editVal);
		//Careful with the overapping of later rects here if it ever changes
		gUISystem.d2dRenderTarget->DrawTextA(editString.c_str(), editString.size(), gUISystem.textFormat, viewRect, gUISystem.brushText);
	}

	POINT mousePos = gUISystem.mousePos;

	if ((mousePos.x > viewRect.left) && (mousePos.x < viewRect.right))
	{
		if ((mousePos.y > viewRect.top) && (mousePos.y < viewRect.bottom))
		{
			gUISystem.d2dRenderTarget->DrawRectangle(viewRect, gUISystem.brushText);

			if (inputSystem.GetMouseLeftUpState())
			{
				gUISystem.activeUIViewElementIndex = idCounter;
			}
		}
	}

	IncrementViewRect();
}
