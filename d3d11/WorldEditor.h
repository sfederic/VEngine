#pragma once

#include "Raycast.h"
#include "Actor.h"

class WorldEditor
{
public:
	void Tick();
	void Init();
	void MoveActor(class Actor* actor);

	std::vector<ActorSystem*> axes;
	ActorSystem xAxis, yAxis, zAxis;

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
