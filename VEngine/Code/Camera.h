#pragma once
#include <DirectXMath.h>

using namespace DirectX;

struct Actor;

struct Camera
{
	XMMATRIX view, proj;

	XMVECTOR position;
	XMVECTOR focusPoint;
	XMVECTOR worldUp;
	XMVECTOR forward, up, right;

	bool editorCamera = false;

	Camera(XMVECTOR startingPosition, bool isEditorCamera);
	void Tick(float deltaTime, int mouseX, int mouseY);
	void UpdateViewMatrix();
	void Pitch(float angle);
	void RotateY(float angle);
	void MouseMove(int x, int y);
	void Move(float d, XMVECTOR axis);
	void ZoomTo(Actor* actor);
	//void FrustumCullTest(ActorSystem& system);
};

extern Camera editorCamera;
extern Camera playerCamera;
extern Camera* activeCamera;
