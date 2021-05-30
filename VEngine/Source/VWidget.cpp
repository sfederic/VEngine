#include "VWidget.h"
#include "UISystem.h"
#include "Input.h"
#include <SpriteBatch.h>
#include <WICTextureLoader.h>
#include "RenderSystem.h"
#include <SimpleMath.h>

void VWidget::Tick(float deltaTime)
{
	Text(L"Hello baby", { 0,0,200,200 });
	Button(L"button", { 200, 200, 300, 300 });
	Image(L"yep");
}

void VWidget::Text(const std::wstring& text, D2D1_RECT_F layout)
{
	gUISystem.d2dRenderTarget->DrawTextA(text.c_str(), text.size(), 
		gUISystem.textFormat, layout, gUISystem.brushText);
}

//Make sure the buttons layout isn't backwards(bottom and top less than left and right)
//else the mouse check won't work.
bool VWidget::Button(const std::wstring& text, D2D1_RECT_F layout, float lineWidth)
{
	gUISystem.d2dRenderTarget->DrawRectangle(layout, gUISystem.brushShapes, lineWidth);
	Text(text, layout);

	if (gUISystem.mousePos.x > layout.left && gUISystem.mousePos.x < layout.right)
	{
		if (gUISystem.mousePos.y > layout.top && gUISystem.mousePos.y < layout.bottom)
		{
			if (gInputSystem.GetMouseLeftUpState())
			{
				return true;
			}
		}
	}

	return false;
}

//REF:https://github.com/Microsoft/DirectXTK/wiki/Sprites-and-textures
void VWidget::Image(const std::wstring& file)
{
	ID3D11ShaderResourceView* m_texture;
	ID3D11Resource* resource;
	HR(CreateWICTextureFromFile(gRenderSystem.device, L"Textures/penguin.png", &resource, &m_texture));

	ID3D11Texture2D* tex;
	resource->QueryInterface(&tex);

	DirectX::SpriteBatch* m_spriteBatch;
	DirectX::SimpleMath::Vector2 m_screenPos;
	DirectX::SimpleMath::Vector2 m_origin;

	m_spriteBatch = new SpriteBatch(gRenderSystem.context);

	m_spriteBatch->Begin();
	m_spriteBatch->Draw(m_texture, m_screenPos, nullptr, Colors::White, 0.f, m_origin);
	m_spriteBatch->End();

	m_texture->Release();
	resource->Release();
	tex->Release();
	delete m_spriteBatch;
}
