#pragma once

#include "Raycast.h"

class WorldEditor
{
public:
	void Tick();
	void MoveActor(class Actor* actor);

	Ray screenPickRay;

	float pickedActorMoveSpeed = 5.f;

	//For translation with mouse
	float dxAccum;
	float dyAccum;

	float moveIncrement = 1.f;

	//For picking
	int actorIndex;
	int actorSystemIndex;

	bool bMoveActorsInIncrements = false;
};

extern WorldEditor gWorldEditor;
