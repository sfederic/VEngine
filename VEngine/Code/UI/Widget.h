#pragma once

#include <string>
#include <DirectXMath.h>
#include "Layout.h"
#include "Core/UID.h"
#include "Colours.h"

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

	virtual ~Widget() = default;
	virtual void Draw(float deltaTime) {}
	virtual void Start() {}
	void Destroy();

	void AddToViewport(float removeTimer = 0.f);
	void OnceOffAddToViewport(float removeTimer);
	void RemoveFromViewport();
	void RemoveFromViewportAndDelete();
	void ToggleInViewport();

	bool IsInViewport();

	bool IsMouseInLayout(Layout layout);

	bool IsStatic() const { return isStaticWidget; }

	void SetWorldPosition(XMVECTOR pos) { originalWorldPosition = pos; }

	auto GetUID() const { return uid; }

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

	void Image(std::string_view filename, Layout layout, float alpha = 1.f);
	Layout ImageAsOriginalSize(std::string_view textureFilename, long x, long y, float alpha = 1.f);
	void Image(std::string_view filename, int x, int y, int w, int h, float alpha = 1.f);
	bool ImageButton(std::string_view filename, Layout layout, float alpha = 1.f);

	void Rect(Layout layout, D2D1_COLOR_F color = Colours::Black, float lineWidth = 1.f, float opacity = 1.f);
	void FillRect(Layout layout, D2D1_COLOR_F color = { 0.5f, 0.5f, 0.5f, 1.f }, float opacity = 1.0f);

	//Aligns widget via enum with specific width and height
	Layout AlignLayout(float w, float h, Align align);

	//Aligns widget to viewport percentage where 0.0 is the top left and 1.0 is the bottom right
	Layout PercentAlignLayout(float left, float top, float right, float bottom);

	Layout CenterLayoutOnScreenSpaceCoords(float w, float h);
	Layout CenterLayoutOnScreenSpaceCoords(float w, float h, float sx, float sy);

	//The widget's position in world space to be mapped to screen space (Use Actor::GetHomogeneousPositionV() to set this)
	XMVECTOR worldPosition = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	XMVECTOR originalWorldPosition = XMVectorSet(0.f, 0.f, 0.f, 1.f);

	UID uid = GenerateUID();

	//Means the widget won't be cleaned up on world resets. Mostly for Debug widgets.
	bool isStaticWidget = false;

public:
	bool render = true;
};
