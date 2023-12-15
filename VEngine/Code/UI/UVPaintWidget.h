#pragma once

#include "Widget.h"

//@Todo: the fucking UISystem keeps cleaning up widgets on swpchain resize.
//make a bool in Widget to denote not to delete shit.

class UVPaintWidget : public Widget
{
public:
	void Draw(float deltaTime) override;

	void SetUVPickerDimensions(float width, float height);
	void SetUVPreviewTextureFilename(std::string_view filename) { uvPreviewTextureFilename = filename; }

private:
	void UpdateWorldEditorUVData();

	std::string uvPreviewTextureFilename = "vagrant_map.png";

	float previousMouseX = 0.f;
	float previousMouseY = 0.f;

	float uvPickerWidth = 100.f;
	float uvPickerHeight = 100.f;
};
