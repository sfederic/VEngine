#pragma once

#include "Raycast.h"

class WorldEditor
{
public:
	void Tick();
	void MoveActor(class Actor* actor);

	Ray screenPickRay;

	float pickedActorMoveSpeed = 5.f;

	//For picking
	int actorIndex;
	int actorSystemIndex;
};

extern WorldEditor gWorldEditor;
