#include "Widget.h"
#include <WICTextureLoader.h>
#include "UI/UISystem.h"
#include "Input.h"
#include "Render/Renderer.h"
#include "Editor/Editor.h"

void Widget::Tick(float deltaTime)
{
	Image(L"test.png", 200.f, 200.f);
	MapToScreenSpace();
}

void Widget::Start()
{
	spriteBatch = new DirectX::SpriteBatch(renderer.context);
}

void Widget::AddToViewport()
{
	uiSystem.AddWidget(this);
}

void Widget::RemoveFromViewport()
{
	uiSystem.RemoveWidget(this);
}

void Widget::MapToScreenSpace()
{
	//What you need to do here it take the actor's position after it's been multiplied 
	//by the MVP matrix on the CPU side of things, divide it by the W component and multiply it out by the viewport.

	//REF:http://www.windows-tech.info/5/a80747e145dd9062.php

	float f1 = pos.m128_f32[0] / pos.m128_f32[3];
	float f2 = pos.m128_f32[1] / pos.m128_f32[3];

	int sx = ((f1 * 0.5f) + 0.5) * renderer.viewport.Width;
	int sy = ((f2 * -0.5f) + 0.5) * renderer.viewport.Height;

	Text(displayText, { (float)sx, (float)sy, (float)sx + 150, (float)sy + 150 });
}

ID3D11ShaderResourceView* Widget::CreateTexture(const std::wstring& filename)
{
	ID3D11ShaderResourceView* textureView;
	std::wstring filepath = L"Textures/" + filename;
	CreateWICTextureFromFile(renderer.device, filepath.c_str(), nullptr, &textureView);
	assert(textureView && "texture filename will be wrong");

	return textureView;
}

void Widget::Text(const std::wstring& text, D2D1_RECT_F layout)
{
	uiSystem.d2dRenderTarget->DrawText(text.c_str(), text.size(),
		uiSystem.textFormat, layout, uiSystem.brushText);
}

//Make sure the buttons layout isn't backwards(bottom and top less than left and right)
//else the mouse check won't work.
bool Widget::Button(const std::wstring& text, D2D1_RECT_F layout, float lineWidth)
{
	uiSystem.d2dRenderTarget->DrawRectangle(layout, uiSystem.brushShapes, lineWidth);
	Text(text, layout);

	if (editor->viewportMouseX > layout.left && editor->viewportMouseY < layout.right)
	{
		if (editor->viewportMouseX > layout.top && editor->viewportMouseY < layout.bottom)
		{
			if (Input::GetMouseLeftUp())
			{
				return true;
			}
		}
	}

	return false;
}

//REF:https://github.com/Microsoft/DirectXTK/wiki/Sprites-and-textures
void Widget::Image(const std::wstring& filename, float x, float y)
{
	//TODO: sprite rendering is fucked up. There's something going wrong with its state changes
	//and maybe its shader binding, models get messed up when this function is called.
	//REF:https://github.com/Microsoft/DirectXTK/wiki/SpriteBatch#state-management
	//REF:https://stackoverflow.com/questions/35558178/directxspritefont-spritebatch-prevents-3d-scene-from-drawing
	auto textureIt = texturesMap.find(filename);
	if (textureIt == texturesMap.end())
	{
		auto texture = CreateTexture(filename);
		texturesMap[filename] = texture;
	}
	else
	{
		DirectX::XMFLOAT2 screenPos(x, y);
		DirectX::XMFLOAT2 origin(0.f, 0.f);

		spriteBatch->Begin();
		spriteBatch->Draw(textureIt->second, screenPos, nullptr, Colors::White, 0.f, origin);
		spriteBatch->End();

		//renderer.context->ClearState();
	}
}
