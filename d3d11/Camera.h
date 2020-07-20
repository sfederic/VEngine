#pragma once

#include <DirectXMath.h>

using namespace DirectX;

class Camera 
{
public:
	Camera() {};
	Camera(XMVECTOR initialLocation); //Set axis to default LH

	void Tick(float deltaTime);
	void UpdateViewMatrix();
	void Strafe(float d);
	void MoveForward(float d);
	void MoveUp(float d);
	void Pitch(float angle);
	void RotateY(float angle);
	void MouseMove(int x, int y);
	void FrustumCullTest(class ActorSystem& system);
	void AttachTo(class Actor* actor);

	XMMATRIX view, proj;

	XMVECTOR location;
	XMVECTOR focusPoint;
	XMVECTOR worldUp;
	XMVECTOR up, right, forward;

	XMVECTOR attachedOffset;
	class Actor* actorAttachedTo;
};

static Camera editorCamera;

Camera* GetPlayerCamera();