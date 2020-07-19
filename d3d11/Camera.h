#pragma once

#include <DirectXMath.h>

using namespace DirectX;

class Camera 
{
public:
	Camera() {};
	Camera(XMVECTOR initialLocation); //Set axis to default LH

	void Tick(class UISystem* ui, class CoreSystem* win32);
	void UpdateViewMatrix();
	void Strafe(float d);
	void MoveForward(float d);
	void MoveUp(float d);
	void Pitch(float angle);
	void RotateY(float angle);
	void MouseMove(int x, int y);
	void FrustumCullTest(class ActorSystem& system);
	void AttachTo(class Actor* actor);

	XMVECTOR location;
	XMVECTOR focusPoint;
	XMVECTOR worldUp;

	XMVECTOR up, right, forward;

	XMMATRIX view, proj;

	class Actor* actorAttachedTo;
	XMVECTOR attachedOffset;
};

static Camera editorCamera;