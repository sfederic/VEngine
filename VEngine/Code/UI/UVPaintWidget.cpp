#include "vpch.h"
#include "UVPaintWidget.h"
#include "Core/Input.h"
#include "Core/WorldEditor.h"
#include "Components/MeshComponent.h"

void UVPaintWidget::Draw(float deltaTime)
{
	__super::Draw(deltaTime);

	SetTextureFromPickedActor();

	const auto imageLayout = ImageAsOriginalSize(uvPreviewTextureFilename, 0, 0);

	auto pickerLayout = Layout({ previousMouseX, previousMouseY, previousMouseX + uvPickerWidth, previousMouseY + uvPickerHeight });

	if (IsMouseInLayout(imageLayout))
	{
		if (Input::GetMouseLeftDown())
		{
			const auto mouseX = (float)Editor::Get().GetViewportMouseX();
			const auto mouseY = (float)Editor::Get().GetViewportMouseY();

			pickerLayout = Layout({ mouseX, mouseY, mouseX + uvPickerWidth, mouseY + uvPickerHeight });

			previousMouseX = mouseX;
			previousMouseY = mouseY;
		}
	}

	Rect(pickerLayout);

	UpdateWorldEditorUVData();
}

void UVPaintWidget::UpdateWorldEditorUVData() const
{
	WorldEditor::uvPaintData.texture = uvPreviewTextureFilename;
	WorldEditor::uvPaintData.x = previousMouseX;
	WorldEditor::uvPaintData.y = previousMouseY;
	WorldEditor::uvPaintData.w = previousMouseX + uvPickerWidth;
	WorldEditor::uvPaintData.h = previousMouseY + uvPickerHeight;
}

void UVPaintWidget::SetTextureFromPickedActor()
{
	auto actor = WorldEditor::GetPickedActor();
	if (actor)
	{
		auto mesh = actor->GetFirstComponentOfTypeAllowNull<MeshComponent>();
		if (mesh)
		{
			uvPreviewTextureFilename = mesh->GetTextureFilename();
		}
	}
}
