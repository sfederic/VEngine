#pragma once

#include <dwrite_1.h>
#include <vector>

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
	void Edit();

	void NewLine();
	void NewLine(int numOfNewlines);
	void IncrementViewRect();
};

struct UIViewActor : public UIView
{
	void Create()
	{
		Init({ 0.f, 0.f, 300.f, 400.f }, L"Properties");
		Text(L"Position");
		Button(L"Test Button");
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

		}

		for (int i = 0; i < buttonItems.size(); i++)
		{
			buttonItems[i].Tick();
		}


	}
};