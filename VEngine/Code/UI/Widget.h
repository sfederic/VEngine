#pragma once

#include <string>
#include <DirectXMath.h>
#include "Layout.h"

using namespace DirectX;

//Base widget class for in-game UI.
class Widget
{
public:
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

	//The widget's position in world space to be mapped to screen space (Use Actor::GetHomogeneousPositionV() to set this)
	XMVECTOR worldPosition = XMVectorSet(0.f, 0.f, 0.f, 1.f);

	bool render = true;

	virtual void Draw(float deltaTime) = 0;
	virtual void Start() {}
	void Destroy();

	void AddToViewport(float removeTimer = 0.f);
	void OnceOffAddToViewport(float removeTimer);
	void RemoveFromViewport();
	void RemoveFromViewportAndDelete();

	bool IsInViewport();

	bool IsMouseInLayout(Layout layout);

protected:
	void GetScreenSpaceCoords(int& sx, int& sy);

	void Text(const std::wstring text, Layout layout, TextAlign align = TextAlign::Center,
		D2D1_COLOR_F color = { 0.f, 0.f, 0.f, 1.f }, float opacity = 1.0f);

	void Text(const std::string text, Layout layout, TextAlign align = TextAlign::Center,
		D2D1_COLOR_F color = { 0.f, 0.f, 0.f, 1.f }, float opacity = 1.0f);

	bool Button(const std::wstring text, Layout layout, float lineWidth = 1.0f,
		TextAlign textAlign = TextAlign::Center, D2D1_COLOR_F textColor = { 0.f, 0.f, 0.f, 1.f }, float textOpacity = 1.0f);

	bool Button(const std::string text, Layout layout, float lineWidth = 1.0f,
		TextAlign textAlign = TextAlign::Center, D2D1_COLOR_F textColor = { 0.f, 0.f, 0.f, 1.f }, float textOpacity = 1.0f);

	void Image(std::string_view filename, Layout layout);
	Layout ImageAsOriginalSize(std::string_view textureFilename, long x, long y);
	void Image(std::string_view filename, int x, int y, int w, int h);
	bool ImageButton(std::string_view filename, Layout layout);

	void Rect(Layout layout);
	void FillRect(Layout layout, D2D1_COLOR_F color = { 0.5f, 0.5f, 0.5f, 1.f }, float opacity = 1.0f);

	//Aligns widget via enum with specific width and height
	Layout AlignLayout(float w, float h, Align align);

	//Aligns widget to viewport percentage where 0.0 is the top left and 1.0 is the bottom right
	Layout PercentAlignLayout(float left, float top, float right, float bottom);

	Layout CenterLayoutOnScreenSpaceCoords(float w, float h);
	Layout CenterLayoutOnScreenSpaceCoords(float w, float h, float sx, float sy);
};
