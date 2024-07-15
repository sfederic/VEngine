#pragma once

#include "Widget.h"

//Displays a reticle to select a segment over the displayed texture.
//Links with WorldEditor to UV paint meshes.
//Is based on works like:
//https://jeiel.itch.io/sprytile
//https://crocotile3d.com/
//https://www.youtube.com/watch?v=vl1uGM-KmY8 (FFTactics)
class UVPaintWidget : public Widget
{
public:
	UVPaintWidget()
	{
		SetTextureFromPickedActor();
		isStaticWidget = true;
	}

	void Draw(float deltaTime) override;

	//Keep these variables public so ImGui can access them easier without the static nonsense.
	std::string uvPreviewTextureFilename;

	float uvPickerWidth = 100.f;
	float uvPickerHeight = 100.f;

private:
	void UpdateWorldEditorUVData() const;
	void SetTextureFromPickedActor();

	float previousMouseX = 0.f;
	float previousMouseY = 0.f;
};
