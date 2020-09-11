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
	float rotateSpeed = 0.f;
	float scaleSpeed = 1.f;

	//For picking
	int actorIndex;
	int actorSystemIndex;

	bool bMoveActorsInIncrements = false;

	bool bRotateActors = false;
	bool bTranslateActors = false;
	bool bScaleActors = false;
};

extern WorldEditor gWorldEditor;
