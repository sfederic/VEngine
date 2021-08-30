#pragma once
#include <DirectXMath.h>
#include <DirectXCollision.h>
#include <fbxsdk/core/math/fbxmatrix.h>

struct Actor;

using namespace DirectX;

namespace VMath
{
	void MatrixAddScale(float s, XMMATRIX& m);

	XMVECTOR XMVectorRight();
	XMVECTOR XMVectorUp();
	XMVECTOR XMVectorForward();

	void RoundFloat3(XMFLOAT3& float3);

	XMFLOAT3 PitchYawRollFromMatrix(XMMATRIX m);
	XMFLOAT3 PitchYawRollFromQuaternion(XMFLOAT4 q);

	void LookAtRotation(XMVECTOR lookAtPoint, XMMATRIX& m);

	XMFLOAT4X4 FbxMatrixToDirectXMathMatrix(fbxsdk::FbxMatrix fbxMatrix);

	bool VecEqual(XMVECTOR v1, XMVECTOR v2, float epsilon = 0.0005f);
	bool Float3Equal(XMFLOAT3& f1, XMFLOAT3& f2);
	float FindMaxInVector(XMVECTOR v);
	XMVECTOR XMVectorConstantLerp(XMVECTOR v1, XMVECTOR v2, float dist);

	XMMATRIX GetBoundingBoxMatrix(BoundingOrientedBox& boundingBox, Actor* actor);
	void UpdateBoundingBox(BoundingOrientedBox& boundingBox, Actor* actor);
}
