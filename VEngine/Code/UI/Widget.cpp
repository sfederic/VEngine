#include "Widget.h"
#include <WICTextureLoader.h>
#include "UI/UISystem.h"
#include "Input.h"
#include "Render/Renderer.h"
#include "Editor/Editor.h"
#include "Render/SpriteSystem.h"

void Widget::Tick(float deltaTime)
{
}

void Widget::Start()
{
}

void Widget::AddToViewport()
{
	uiSystem.AddWidget(this);
}

void Widget::RemoveFromViewport()
{
	uiSystem.RemoveWidget(this);
}

void Widget::GetScreenSpaceCoords(int& sx, int& sy)
{
	//What you need to do here it take the actor's position after it's been multiplied 
	//by the MVP matrix on the CPU side of things, divide it by the W component 
	//and multiply it out by the viewport.
	//REF:http://www.windows-tech.info/5/a80747e145dd9062.php

	const float f1 = pos.m128_f32[0] / pos.m128_f32[3];
	const float f2 = pos.m128_f32[1] / pos.m128_f32[3];

	sx = ((f1 * 0.5f) + 0.5) * renderer.viewport.Width;
	sy = ((f2 * -0.5f) + 0.5) * renderer.viewport.Height;
}

void Widget::Text(const std::wstring& text, D2D1_RECT_F layout)
{
	uiSystem.d2dRenderTarget->DrawText(text.c_str(), text.size(),
		uiSystem.textFormat, layout, uiSystem.brushText);
}

bool Widget::Button(const std::wstring& text, D2D1_RECT_F layout, float lineWidth)
{
	uiSystem.d2dRenderTarget->DrawRectangle(layout, uiSystem.brushShapes, lineWidth);
	Text(text, layout);

	if (Input::GetMouseLeftUp())
	{
		if (editor->viewportMouseX > layout.left && editor->viewportMouseX < layout.right)
		{
			if (editor->viewportMouseY > layout.top && editor->viewportMouseY < layout.bottom)
			{
				return true;
			}
		}
	}

	return false;
}

void Widget::Image(const std::string& filename, int x, int y, int w, int h)
{
	Sprite sprite = {};
	sprite.textureFilename = filename;
	sprite.dstRect = { x, y, x + w, y + h };
	sprite.srcRect = { 0, 0, w, h };

	spriteSystem.CreateScreenSprite(sprite);
}

void Widget::Rect(D2D1_RECT_F layout)
{
	uiSystem.d2dRenderTarget->FillRectangle(layout, uiSystem.brushShapes);
}

D2D1_RECT_F Widget::AlignLayout(float w, float h, Align align)
{
	float vw = renderer.GetViewportWidth();
	float vh = renderer.GetViewportHeight();

	switch (align)
	{
	case Align::Center: 
		vw /= 2.f;
		vh /= 2.f;
		break;

	case Align::Right:
		vw *= 0.75;
		vh /= 2.f;
		break;

	case Align::Left:
		vw *= 0.25f;
		vh /= 2.f;
		break;

	case Align::Top:
		vw /= 2.f;
		vh *= 0.25f;
		break;

	case Align::Bottom:
		vw /= 2.f;
		vh *= 0.75f;
		break;

	case Align::TopLeft:
		vw *= 0.25f;
		vh *= 0.25f;
		break;

	case Align::TopRight:
		vw *= 0.75f;
		vh *= 0.25f;
		break;

	case Align::BottomLeft:
		vw *= 0.25f;
		vh *= 0.75f;
		break;

	case Align::BottomRight:
		vw *= 0.75f;
		vh *= 0.75f;
		break;
	}

	D2D1_RECT_F rect = { vw - w, vh - h, vw + w, vh + h };
	return rect;
}
