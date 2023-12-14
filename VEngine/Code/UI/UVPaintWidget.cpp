#include "vpch.h"
#include "UVPaintWidget.h"
#include "Core/Input.h"

void UVPaintWidget::Draw(float deltaTime)
{
	const auto layout = PercentAlignLayout(0.1f, 0.1f, 0.5f, 0.5f);

	ImageAsOriginalSize("vagrant_map.png", 5, 5);

	auto rectLayout = Layout({ previousMouseX, previousMouseY, previousMouseX + 100.f, previousMouseY + 100.f });

	if (Input::GetMouseLeftUp())
	{
		const auto mouseX = (float)editor->GetViewportMouseX();
		const auto mouseY = (float)editor->GetViewportMouseY();

		rectLayout = Layout({ mouseX, mouseY, mouseX + 100.f, mouseY + 100.f });

		previousMouseX = mouseX;
		previousMouseY = mouseY;
	}

	Rect(rectLayout);
}
