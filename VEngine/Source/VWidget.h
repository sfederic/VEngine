#pragma once

#include <string>

//Base widget class for in-game UI
class VWidget
{
public:
	virtual void Tick(float deltaTime);

	void Text(const std::wstring& text);

	bool bRender = true;
};
