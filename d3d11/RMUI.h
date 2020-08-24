#pragma once

#include <dwrite_1.h>
#include <vector>
#include <string>
#include <d2d1_1.h>

struct Widget
{
	D2D1_RECT_F viewRect;
};

struct RMText : Widget
{
	void Tick(const wchar_t* string);
};

class RMUISystem
{
public:
	
};

