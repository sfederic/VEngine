#pragma once

#include <DirectXMath.h>
#include <fbxsdk/core/math/fbxmatrix.h>

using namespace DirectX;

void MatrixAddScale(float s, XMMATRIX& m);
XMVECTOR XMVectorRight();
XMVECTOR XMVectorUp();
XMVECTOR XMVectorForward();
//XMVECTOR XMVectorConstantLerp(FXMVECTOR V0, FXMVECTOR V1, float deltaTime, float speed);
XMFLOAT3 PitchYawRollFromMatrix(XMMATRIX m);
XMFLOAT3 PitchYawRollFromQuaternion(XMFLOAT4 q);
void LookAtRotation(XMVECTOR lookAtPoint, XMMATRIX& m);
XMFLOAT4X4 FbxMatrixToDirectXMathMatrix(fbxsdk::FbxMatrix fbxMatrix);
bool VecEqual(XMVECTOR v1, XMVECTOR v2, float epsilon = 0.0005f);
XMVECTOR XMVectorConstantLerp(XMVECTOR v1, XMVECTOR v2, float dist);