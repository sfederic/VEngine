#pragma once

#include <DirectXMath.h>

using namespace DirectX;

class Camera
{
public:
	Camera() {};
	Camera(XMVECTOR initialLocation);

	void Tick(float deltaTime);
	void UpdateViewMatrix();
	void Pitch(float angle);
	void RotateY(float angle);
	void MouseMove(int x, int y);
	void FrustumCullTest(class ActorSystem& system);
	void AttachTo(class Actor* actor);
	void Move(float d, XMVECTOR axis);
	void ZoomTo(class Actor* actor);

	XMMATRIX view, proj;

	XMVECTOR location;
	XMVECTOR focusPoint;
	XMVECTOR worldUp;
	XMVECTOR forward, up, right;

	XMVECTOR attachedOffset;
	class Actor* actorAttachedTo = nullptr;
};

extern Camera editorCamera;
extern Camera playerCamera;

Camera* GetActiveCamera();
void SetActiveCamera(Camera* camera);