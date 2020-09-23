#pragma once

#include <DirectXMath.h>

using namespace DirectX;

void MatrixAddScale(float s, XMMATRIX& m);
XMVECTOR XMVectorRight();
XMVECTOR XMVectorUp();
XMVECTOR XMVectorForward();
//XMVECTOR XMVectorConstantLerp(FXMVECTOR V0, FXMVECTOR V1, float deltaTime, float speed);
XMVECTOR RollPitchYawFromMatrix(XMMATRIX m);