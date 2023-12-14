#include "vpch.h"
#include "UVPaintWidget.h"
#include "Core/Input.h"
#include "Core/WorldEditor.h"

void UVPaintWidget::Draw(float deltaTime)
{
	const auto imageLayout = ImageAsOriginalSize("vagrant_map.png", 0, 0);

	auto pickerLayout = Layout({ previousMouseX, previousMouseY, previousMouseX + 100.f, previousMouseY + 100.f });

	if (IsMouseInLayout(imageLayout))
	{
		if (Input::GetMouseLeftDown())
		{
			const auto mouseX = (float)editor->GetViewportMouseX();
			const auto mouseY = (float)editor->GetViewportMouseY();

			pickerLayout = Layout({ mouseX, mouseY, mouseX + 100.f, mouseY + 100.f });

			previousMouseX = mouseX;
			previousMouseY = mouseY;
		}
	}

	Rect(pickerLayout);

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
