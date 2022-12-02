#pragma once

#include <DirectXMath.h>

using namespace DirectX;

class PlayerUnit;

//Input controller for attaching to player and monster units, allowing input and movement.
class PlayerInputController
{
public:
	void Tick(float deltaTime);

	PlayerUnit* playerUnitToControl = nullptr;

private:
	void CheckNextMoveNode(XMVECTOR previousPos);
};
