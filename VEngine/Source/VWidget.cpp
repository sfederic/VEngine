#include "VWidget.h"
#include "UISystem.h"
#include "Input.h"
#include <WICTextureLoader.h>
#include "RenderSystem.h"

void VWidget::Tick(float deltaTime)
{
	Text(L"Hello baby", { 0,0,200,200 });
	Button(L"button", { 200, 200, 300, 300 });
	Image(texture1, 200, 200);
}

void VWidget::Start()
{
	spriteBatch = new DirectX::SpriteBatch(gRenderSystem.context);

	texture1 = CreateTexture(L"penguin.png");
}

ID3D11ShaderResourceView* VWidget::CreateTexture(const std::wstring& filename)
{
	ID3D11ShaderResourceView* textureView;
	std::wstring filepath = L"Textures/" + filename;
	CreateWICTextureFromFile(gRenderSystem.device, filepath.c_str(), nullptr, &textureView);
	assert(textureView && "texture filename will be wrong");

	return textureView;
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
void VWidget::Image(ID3D11ShaderResourceView* texture, float x, float y)
{
	DirectX::XMFLOAT2 screenPos(x, y);
	DirectX::XMFLOAT2 origin(0.f, 0.f);

	spriteBatch->Begin();
	spriteBatch->Draw(texture, screenPos, nullptr, Colors::White, 0.f, origin);
	spriteBatch->End();
}
