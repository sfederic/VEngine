#pragma once

#include <string>
#include <d2d1_1.h>

//Base widget class for in-game UI
class VWidget
{
public:
	virtual void Tick(float deltaTime);

	void Text(const std::wstring& text, D2D1_RECT_F layout);
	void Button(const std::wstring& text, D2D1_RECT_F layout, float lineWidth = 1.0f);

	bool bRender = true;
};
