#include "vpch.h"
#include "Widget.h"
#include "Core/Input.h"
#include "Render/Renderer.h"
#include "Editor/Editor.h"
#include "Render/SpriteSystem.h"
#include "Core/Timer.h"
#include "Core/VString.h"
#include "Core/VMath.h"
#include "UISystem.h"
#include "Render/TextureSystem.h"
#include "Render/Texture2D.h"

void Widget::Draw(float deltaTime)
{
	worldPosition = VMath::ConvertToHomogenousPosition(originalWorldPosition);
}

void Widget::Destroy()
{
	RemoveFromViewport();
	UISystem::DestroyWidget(this);
}

void Widget::AddToViewport(float removeTimer)
{
	UISystem::AddWidget(this);

	if (removeTimer > 0.f)
	{
		Timer::SetTimer(removeTimer, std::bind(&Widget::RemoveFromViewport, this));
	}
}

void Widget::OnceOffAddToViewport(float removeTimer)
{
	UISystem::AddWidget(this);
	assert(removeTimer > 0.f);
	Timer::SetTimer(removeTimer, std::bind(&Widget::RemoveFromViewportAndDelete, this));
}

void Widget::RemoveFromViewport()
{
	UISystem::RemoveWidget(this);
}

void Widget::RemoveFromViewportAndDelete()
{
	RemoveFromViewport();
	UISystem::DestroyWidget(this);
}

void Widget::ToggleInViewport()
{
	if (IsInViewport())
	{
		RemoveFromViewport();
	}
	else
	{
		AddToViewport();
	}
}

bool Widget::IsInViewport()
{
	for (Widget* widget : UISystem::widgetsInViewport)
	{
		if (widget == this)
		{
			return true;
		}
	}

	return false;
}

bool Widget::IsMouseInLayout(Layout layout)
{
	const int viewportMouseX = Editor::Get().GetViewportMouseX();
	const int viewportMouseY = Editor::Get().GetViewportMouseY();

	if (viewportMouseX > layout.rect.left && viewportMouseX < layout.rect.right)
	{
		if (viewportMouseY > layout.rect.top && viewportMouseY < layout.rect.bottom)
		{
			return true;
		}
	}

	return false;
}

void Widget::GetScreenSpaceCoords(int& sx, int& sy)
{
	VMath::HomogenousWorldPosToScreenSpaceCoords(worldPosition, sx, sy);
}

void Widget::Text(const std::wstring text, Layout layout, TextAlign align, D2D1_COLOR_F color, float opacity)
{
	DWRITE_TEXT_ALIGNMENT endAlignment{};

	switch (align)
	{
	case TextAlign::Center:
		endAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
		break;

	case TextAlign::Justified:
		endAlignment = DWRITE_TEXT_ALIGNMENT_JUSTIFIED;
		break;

	case TextAlign::Leading:
		endAlignment = DWRITE_TEXT_ALIGNMENT_LEADING;
		break;

	case TextAlign::Trailing:
		endAlignment = DWRITE_TEXT_ALIGNMENT_TRAILING;
		break;
	}

	UISystem::TextDraw(text, layout, endAlignment, color, opacity);
}

void Widget::Text(const std::string text, Layout layout, TextAlign align, D2D1_COLOR_F color, float opacity)
{
	Text(VString::stows(text), layout, align, color, opacity);
}

bool Widget::Button(const std::wstring text, Layout layout, float lineWidth,
	TextAlign textAlign, D2D1_COLOR_F textColor, float textOpacity)
{
	UISystem::FillRect(layout, Colours::Grey);
	Text(text, layout.rect, textAlign, textColor, textOpacity);

	const int viewportMouseX = Editor::Get().GetViewportMouseX();
	const int viewportMouseY = Editor::Get().GetViewportMouseY();

	if (viewportMouseX > layout.rect.left && viewportMouseX < layout.rect.right)
	{
		if (viewportMouseY > layout.rect.top && viewportMouseY < layout.rect.bottom)
		{
			//Hover animation/image
			UISystem::DrawRect(layout, Colours::Black, 2.0f);

			if (Input::GetMouseLeftUp())
			{
				return true;
			}
		}
	}

	return false;
}

bool Widget::Button(const std::string text, Layout layout, float lineWidth, TextAlign textAlign, D2D1_COLOR_F textColor, float textOpacity)
{
	return Button(VString::stows(text), layout, lineWidth, textAlign, textColor, textOpacity);
}

void Widget::Image(std::string_view filename, Layout layout, float alpha)
{
	Sprite sprite = {};
	sprite.textureFilename = filename;
	sprite.dstRect = { (long)layout.rect.left, (long)layout.rect.top, (long)layout.rect.right, (long)layout.rect.bottom };
	sprite.srcRect = { 0, 0, (long)layout.rect.right, (long)layout.rect.bottom };
	sprite.colour.w = alpha;

	SpriteSystem::CreateScreenSprite(sprite);
}

Layout Widget::ImageAsOriginalSize(std::string_view textureFilename, long x, long y, float alpha)
{
	Sprite sprite = {};
	sprite.textureFilename = textureFilename;
	sprite.colour.w = alpha;

	const auto texture = TextureSystem::FindTexture2D(sprite.textureFilename);
	const auto texWidth = texture->GetWidth();
	const auto texHeight = texture->GetHeight();

	sprite.dstRect = { x, y, x + (long)texWidth, y + (long)texHeight };
	sprite.srcRect = { 0, 0, (long)texWidth, (long)texHeight };

	SpriteSystem::CreateScreenSprite(sprite);

	Layout imageLayout = {};
	imageLayout.rect.left = (float)x;
	imageLayout.rect.top = (float)y;
	imageLayout.rect.right = (float)texWidth;
	imageLayout.rect.bottom = (float)texHeight;
	return imageLayout;
}

void Widget::Image(std::string_view filename, int x, int y, int w, int h, float alpha)
{
	Sprite sprite = {};
	sprite.textureFilename = filename;
	sprite.dstRect = { x, y, x + w, y + h };
	sprite.srcRect = { 0, 0, w, h };
	sprite.colour.w = alpha;

	SpriteSystem::CreateScreenSprite(sprite);
}

bool Widget::ImageButton(std::string_view filename, Layout layout, float alpha)
{
	Sprite sprite = {};
	sprite.textureFilename = filename;
	sprite.dstRect = { (long)layout.rect.left, (long)layout.rect.top, (long)layout.rect.right, (long)layout.rect.bottom };
	sprite.srcRect = { 0, 0, (long)layout.rect.right, (long)layout.rect.bottom };
	sprite.colour.w = alpha;

	SpriteSystem::CreateScreenSprite(sprite);

	const int viewportMouseX = Editor::Get().GetViewportMouseX();
	const int viewportMouseY = Editor::Get().GetViewportMouseY();

	if (viewportMouseX > layout.rect.left && viewportMouseX < layout.rect.right)
	{
		if (viewportMouseY > layout.rect.top && viewportMouseY < layout.rect.bottom)
		{
			//Hover animation/image
			UISystem::DrawRect(layout, Colours::Black, 2.f);

			if (Input::GetMouseLeftUp())
			{
				return true;
			}
		}
	}

	return false;
}

void Widget::Rect(Layout layout, D2D1_COLOR_F color, float lineWidth, float opacity)
{
	UISystem::DrawRect(layout, color, lineWidth, opacity);
}

void Widget::FillRect(Layout layout, D2D1_COLOR_F color, float opacity)
{
	UISystem::FillRect(layout, color, opacity);
}

Layout Widget::AlignLayout(float w, float h, Align align)
{
	float vw = Renderer::GetViewportWidth();
	float vh = Renderer::GetViewportHeight();

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

	if (rect.left < 0.f) rect.left = 0.f;
	if (rect.top < 0.f) rect.top = 0.f;
	if (rect.right > Renderer::GetViewportWidth()) rect.right = vw;
	if (rect.bottom > Renderer::GetViewportHeight()) rect.bottom = vh;

	Layout layout = {};
	layout.rect = rect;

	return layout;
}

Layout Widget::PercentAlignLayout(float left, float top, float right, float bottom)
{
	float vw = Renderer::GetViewportWidth();
	float vh = Renderer::GetViewportHeight();

	float endLeft = vw * left;
	float endTop = vh * top;
	float endRight = vw * right;
	float endBottom = vh * bottom;
	D2D1_RECT_F rect = { endLeft, endTop, endRight, endBottom };

	if (rect.left < 0.f) rect.left = 0.f;
	if (rect.top < 0.f) rect.top = 0.f;
	if (rect.right > Renderer::GetViewportWidth()) rect.right = vw;
	if (rect.bottom > Renderer::GetViewportHeight()) rect.bottom = vh;

	return rect;
}

Layout Widget::CenterLayoutOnScreenSpaceCoords(float w, float h)
{
	int sx = 0, sy = 0;
	GetScreenSpaceCoords(sx, sy);

	return CenterLayoutOnScreenSpaceCoords(w, h, sx, sy);
}

Layout Widget::CenterLayoutOnScreenSpaceCoords(float w, float h, float sx, float sy)
{
	D2D1_RECT_F rect = { sx - w, sy - h, sx + w, sy + h };

	float vw = Renderer::GetViewportWidth();
	float vh = Renderer::GetViewportHeight();

	if (rect.left < 0.f) rect.left = 0.f;
	if (rect.top < 0.f) rect.top = 0.f;
	if (rect.right > vw) rect.right = vw;
	if (rect.bottom > vh) rect.bottom = vh;

	return Layout(rect);
}
