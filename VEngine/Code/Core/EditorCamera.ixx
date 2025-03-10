#include <DirectXMath.h>

export module Core.EditorCamera;

import Components.CameraComponent;

export class EditorCamera : public CameraComponent
{
public:
	EditorCamera(DirectX::XMFLOAT3 startPos) : CameraComponent(startPos) {}
	void Tick(float deltaTime) override;
	DirectX::XMMATRIX GetViewMatrix();

private:
	void MouseMove(int x, int y);

	DirectX::XMINT2 lastMousePos{};
	bool arcBallMovementOn = false;
};
