#pragma once

#include "Raycast.h"
#include "TranslationGizmo.h"

class WorldEditor
{
public:
	void Tick(ID3D11Buffer* debugLinesBuffer, class EditorMainWindow* editorMainWindow);
	void Init();
	void MoveActor(Actor* actor, XMVECTOR direction);
	void MoveActor(class Actor* actor, EPickedAxis axis);

	TranslationGizmoSystem translationGizmos;

	Ray screenPickRay;

	Actor* pickedActor;
	TranslationGizmo* pickedAxis;
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
