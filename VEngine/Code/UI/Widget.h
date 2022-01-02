#pragma once
#include <string>
#include <d2d1_1.h>
#include <unordered_map>
#include <DirectXMath.h>
#include "UISystem.h"

using namespace DirectX;

//I can never get the D2D1_RECT_F things right
//left = The x-coordinate of the upper-left corner of the rectangle
//top = The y-coordinate of the upper-left corner of the rectangle.
//right = The x-coordinate of the lower-right corner of the rectangle
//bottom = The y-coordinate of the lower-right corner of the rectangle.

struct Layout
{
	D2D1_RECT_F rect = {};

	Layout() {}
	Layout(D2D1_RECT_F rect_) : rect(rect_) {}

	inline void AddVerticalSpace(float spacing)
	{
		rect.top += spacing;
		rect.bottom += spacing;
	}

	inline void AddHorizontalSpace(float spacing)
	{
		rect.left += spacing;
		rect.right += spacing;
	}

	inline void PushToLeft(float spacing = 0.f)
	{
		rect.right = rect.left + spacing;
	}

	inline void PushToTop(float spacing = 0.f)
	{
		rect.bottom = rect.top + spacing;
	}
};

//Base widget class for in-game UI.
struct Widget
{
	enum class TextAlign
	{
		Center,
		Justified,
		Leading,
		Trailing
	};

	enum class Align
	{
		Center,
		Right,
		Left,
		Bottom,
		Top,
		TopLeft,
		TopRight,
		BottomLeft,
		BottomRight,
	};

	//pos is the widget's position in world space to be mapped to screen space.
	XMVECTOR pos;

	bool render = true;

	virtual void Draw(float deltaTime);
	virtual void Start();

	void AddToViewport(float removeTimer = 0.f);
	void RemoveFromViewport();
	void GetScreenSpaceCoords(int& sx, int& sy);

	void Text(const std::wstring& text, Layout layout, TextAlign align = TextAlign::Center,
		D2D1_COLOR_F color = { 0.f, 0.f, 0.f, 1.f }, float opacity = 1.0f);

	bool Button(const std::wstring& text, Layout layout, float lineWidth = 1.0f,
		TextAlign textAlign = TextAlign::Center, D2D1_COLOR_F textColor = {0.f, 0.f, 0.f, 1.f}, float textOpacity = 1.0f);

	void Image(const std::string& filename, Layout layout);
	void Image(const std::string& filename, int x, int y, int w, int h);

	void Rect(Layout layout);
	void FillRect(Layout layout, D2D1_COLOR_F color = {0.5f, 0.5f, 0.5f, 1.f}, float opacity = 1.0f);

	//Aligns widget via enum with specific width and height
	Layout AlignLayout(float w, float h, Align align);

	//Aligns widget to viewport percentage where 0.0 is the top left and 1.0 is the bottom right
	Layout PercentAlignLayout(float left, float top, float right, float bottom);

	Layout CenterLayoutOnScreenSpaceCoords(float w, float h, float sx, float sy);
};

template <typename T>
T* CreateWidget()
{
	T* widget = uiSystem.CreateWidget<T>();
	return widget;
}
