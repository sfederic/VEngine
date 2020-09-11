#pragma once

#include "Raycast.h"
#include "Actor.h"

class WorldEditor
{
public:
	void Tick(ID3D11Buffer* debugLinesBuffer, class EditorMainWindow* editorMainWindow);
	void Init();
	void MoveActor(Actor* actor, XMVECTOR direction);
	void MoveActor(class Actor* actor, PickedAxis axis);

	std::vector<ActorSystem*> axes;
	ActorSystem xAxis, yAxis, zAxis;

	Ray screenPickRay;

	Actor* pickedActor;
	Actor* pickedAxis;
	XMVECTOR pickedDirection;

	float pickedActorMoveSpeed = 5.f;

	//For translation with mouse
	float dxAccum;
	float dyAccum;
	float dzAccum;

	float lastMousePosX;
	float lastMousePosY;

	float moveIncrement = 2.f;

	//For picking
	int actorIndex;
	int actorSystemIndex;

	bool bMoveActorsInIncrements = false;
};

extern WorldEditor gWorldEditor;
