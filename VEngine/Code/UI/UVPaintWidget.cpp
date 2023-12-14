#include "vpch.h"
#include "UVPaintWidget.h"
#include "Core/Input.h"
#include "Core/WorldEditor.h"

void UVPaintWidget::Draw(float deltaTime)
{
	const auto layout = PercentAlignLayout(0.1f, 0.1f, 0.5f, 0.5f);

	ImageAsOriginalSize("vagrant_map.png", 0, 0);

	auto rectLayout = Layout({ previousMouseX, previousMouseY, previousMouseX + 100.f, previousMouseY + 100.f });

	//@Todo: this might need to change to only click when inside the image
	if (Input::GetMouseMiddleDown())
	{
		const auto mouseX = (float)editor->GetViewportMouseX();
		const auto mouseY = (float)editor->GetViewportMouseY();

		rectLayout = Layout({ mouseX, mouseY, mouseX + 100.f, mouseY + 100.f });

		previousMouseX = mouseX;
		previousMouseY = mouseY;
	}

	Rect(rectLayout);

	UpdateWorldEditor();
}

void UVPaintWidget::UpdateWorldEditor()
{
	WorldEditor::uvPaintData.texture = "vagrant_map.png";
	WorldEditor::uvPaintData.x = previousMouseX;
	WorldEditor::uvPaintData.y = previousMouseY;
	WorldEditor::uvPaintData.w = previousMouseX + 100.f;
	WorldEditor::uvPaintData.h = previousMouseY + 100.f;
}
