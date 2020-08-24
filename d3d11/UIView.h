#pragma once

#include <dwrite_1.h>
#include <vector>
#include <string>
#include "CoreSystem.h"

//base struct for UI elements to inherit from
struct UIView
{
	D2D1_RECT_F viewRectBack; //The backdrop rect for the entire view
	D2D1_RECT_F viewRect; //The layout rect for each element within the view

	int viewId;
	int idCounter = 0; 

	bool bIsHidden;

	void Begin(D2D1_RECT_F viewRect_, const wchar_t* title);
	void End();
	
	virtual void Tick() {}

	void Text(const wchar_t* string);
	void TextClick(const wchar_t* string);
	bool Button(const wchar_t* string);
	void CheckBox(const wchar_t* string, bool& checkBoxVal);
	
	void ListView(std::vector<std::wstring>& strings);

	template <typename T>
	void Edit(T& editVal, std::wstring& editString);

	void NewLine();
	void NewLine(int numOfNewlines);
	void SetSpacing(float val);
	void IncrementViewRectAndID();
};

struct PropertyView
{
	
};

struct TestUIView : public UIView
{
	std::vector<std::wstring> strings;

	void Create()
	{
		strings.push_back(L"Entity1");
		strings.push_back(L"Entity2");
		strings.push_back(L"Entity2");
		strings.push_back(L"Entity2");
		strings.push_back(L"Entity2");
		strings.push_back(L"Entity2");
		strings.push_back(L"Entity2");
		strings.push_back(L"Entity2");
	}

	virtual void Tick()
	{
		Begin({ 0.f, 0.f, 200.f, 400.f }, L"Testing");
		ListView(strings);
		End();
	}
};

//TODO: See if throwing this into another .h file will lower compile times
struct UIViewActor : public UIView
{
	virtual void Tick();

	std::wstring posStringX;
	std::wstring posStringY;
	std::wstring posStringZ;

	std::wstring scaleStringX;
	std::wstring scaleStringY;
	std::wstring scaleStringZ;

	std::wstring rotStringX;
	std::wstring rotStringY;
	std::wstring rotStringZ;
};

//TODO: Figrue out whether this sort of template work would just be better of with overloaded functions. 
//eg. Editfloat, EditInt, EditString
template<typename T>
inline void UIView::Edit(T& editVal, std::wstring& editString)
{
	POINT mousePos = gUISystem.mousePos;

	if ((mousePos.x > viewRect.left) && (mousePos.x < viewRect.right))
	{
		if ((mousePos.y > viewRect.top) && (mousePos.y < viewRect.bottom))
		{
			gUISystem.d2dRenderTarget->DrawRectangle(viewRect, gUISystem.brushText);

			if (gInputSystem.GetMouseLeftUpState())
			{
				gUISystem.activeUIViewElementIndex = idCounter;
			}
		}
	}

	if (gUISystem.activeUIViewElementIndex == idCounter)
	{
		if (gInputSystem.GetKeyUpState(VK_TAB))
		{
			gUISystem.activeUIViewElementIndex++;
		}

		gUISystem.bEditUIActive = true;

		gUISystem.d2dRenderTarget->DrawRectangle(viewRect, gUISystem.brushTextBlack);

		//HANDLE TEXT INPUT
		if (gInputSystem.GetAsyncKey(VK_CONTROL)) //Delete all characters
		{
			if (gInputSystem.GetKeyDownState(VK_BACK))
			{
				editString.clear();
			}
		}
		else if (gInputSystem.GetKeyDownState(VK_BACK)) //Backspace
		{
			if (editString.size() > 0)
			{
				editString.pop_back();
			}
		}
		else if (gInputSystem.GetKeyDownState(VK_RETURN)) //Enter value
		{
			editVal = (T)_wtof(editString.c_str());
			editString = std::to_wstring(editVal);

			gUISystem.activeUIViewElementIndex = -1;
			gUISystem.bEditUIActive = false;
		}
		else if (gInputSystem.GetKeyDownState(VK_OEM_PERIOD)) //(./>) key
		{
			editString.push_back('.');
		}
		else if (gInputSystem.GetAnyKeyDown()) //Careful with the else-if here on GetAnyKey
		{
			const int maxEditStringSize = 64;
			if (editString.size() < maxEditStringSize)
			{
				wchar_t key = gInputSystem.currentDownKey;
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

	IncrementViewRectAndID();
}
