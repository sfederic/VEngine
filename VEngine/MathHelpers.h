#pragma once

#include <DirectXMath.h>

using namespace DirectX;

void MatrixAddScale(float s, XMMATRIX& m);
XMVECTOR XMVectorRight();
XMVECTOR XMVectorUp();
XMVECTOR XMVectorForward();
XMVECTOR RollPitchYawFromMatrix(XMMATRIX m);