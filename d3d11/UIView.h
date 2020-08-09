#pragma once

#include <dwrite_1.h>
#include <vector>
#include <string>

//Vernacular: Item = edit box, button, etc.
//View = entire structure

struct UIItem
{
	D2D1_RECT_F rect;
	int id;
};

struct EditItem : public UIItem
{
	void Tick();

	wchar_t string[64];
};

struct ButtonItem : public UIItem
{
	void Tick();

	wchar_t string[64];
};

struct TextItem : public UIItem
{
	void Tick();

	wchar_t string[64];
};

//base struct for UI elements to inherit from
struct UIView
{
	D2D1_RECT_F viewRectBack;
	D2D1_RECT_F viewRect;
	D2D1_RECT_F viewRectOffset;

	std::vector<EditItem> editItems;
	std::vector<TextItem> textItems;
	std::vector<ButtonItem> buttonItems;
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

struct UIViewActor : public UIView
{
	void Create()
	{
		//Init({ 0.f, 0.f, 300.f, 400.f }, L"Properties");
		//Text(L"Position");
		//Button(L"Test Button");
	}

	void RenderBack(const wchar_t* title);

	virtual void Tick()
	{
		/*Init({ 0.f, 0.f, 300.f, 400.f }, L"Properties");
		Text(L"Position");
		Edit();
		Edit();
		Text(L"Rotation");
		Text(L"Scale");
		Button(L"Test BUtton");*/

		RenderBack(L"Properties");

		for (int i = 0; i < textItems.size(); i++)
		{
			textItems[i].Tick();
		}

		for (int i = 0; i < editItems.size(); i++)
		{
			editItems[i].Tick();
		}

		for (int i = 0; i < buttonItems.size(); i++)
		{
			buttonItems[i].Tick();
		}
	}
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
