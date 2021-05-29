#pragma once

#include "Raycast.h"
#include "Actor.h"

class WorldEditor
{
public:
	void Tick(ID3D11Buffer* debugLinesBuffer);
	void Init();
	void DeleteActor();
	void DuplicateActor();
	void SetPickedActor(Actor* actor);

	Actor* pickedActor;

	float lastMousePosX;
	float lastMousePosY;

	bool bMoveActorsInIncrements = false;
	bool bRotateActors = false;
	bool bTranslateActors = false;
	bool bScaleActors = false;
};

extern WorldEditor gWorldEditor;
