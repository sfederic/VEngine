#pragma once

#include "Widget.h"

//@Todo: the fucking UISystem keeps cleaning up widgets on swap chain resize.
//make a bool in Widget to denote not to delete shit.

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

//@Todo: One workflow idea these sort of PS1 JRPG titles have (or at least what I thought would be good)
//is to be able to paint textures directly onto faces. Everything in the engine is triangulated but the idea 
//is based on works like:
//https://jeiel.itch.io/sprytile
//https://crocotile3d.com/
//What those two example are missing though is directly painting to existing faces instead of creating quads from
//the selected area of the texture map.

//What this tool would need is a picker that could be resized to various dimensions (e.g. 1x16, 4x4, 8x4, etc.)
//with the texture map image loaded, and then able to map the selected triangles to the texture map picker.

//The workflow currently in Blender isn't too bad, but this sort of tool is worth looking into down the line.
//The hardest part would be trying to map two triangles and orient them into a single quad so that the texture
//picker would line up. The output would have to remape the UVs to the selcted mesh's .vmesh file, which wouldn't 
//show up in the original .fbx export which is a worry.

//Note that this tool might be easier to go in ImGui instead of Qt. Maybe even Direct2D.
//Try use the FFTactics https://www.youtube.com/watch?v=vl1uGM-KmY8 tools as a reference too.

//IMPLEMENTATION NOTES AND IDEAS
//Trying to think on how to do this, I'm thinking have this texture map reticle/picker be split into two triangles.
//So the left click will asign the selected mesh face to the top left triangle, and the right click will assign the
//bottom right. Should be able to rotate this sort of setup to offer more flexibility.
