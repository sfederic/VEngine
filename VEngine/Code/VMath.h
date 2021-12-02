#pragma once
#include <DirectXMath.h>
#include <DirectXCollision.h>
#include <fbxsdk/core/math/fbxmatrix.h>

struct Actor;
struct Transform;
struct SpatialComponent;

using namespace DirectX;

//It's called VMath because the C libs fuck around with the includes
namespace VMath
{
	void MatrixAddScale(float s, XMMATRIX& m);

	XMVECTOR XMVectorRight();
	XMVECTOR XMVectorUp();
	XMVECTOR XMVectorForward();

	//returns matrix with all 0's
	XMMATRIX ZeroMatrix();

	void RoundFloat3(XMFLOAT3& float3);

	XMFLOAT3 PitchYawRollFromMatrix(XMMATRIX m);
	XMFLOAT3 PitchYawRollFromQuaternion(XMFLOAT4 q);

	void LookAtRotation(XMVECTOR lookAtPoint, XMMATRIX& m);

	//Rotates on the Y-axis
	void RotateTowardsCamera(Transform& transform);

	XMFLOAT4X4 FbxMatrixToDirectXMathMatrix(fbxsdk::FbxMatrix fbxMatrix);

	//bool VecEqual(XMVECTOR v1, XMVECTOR v2, float epsilon = 0.0005f);
	bool VecEqual(XMVECTOR v1, XMVECTOR v2, float epsilon = 9.9999999747e-05f);
	bool Float3Equal(XMFLOAT3& f1, XMFLOAT3& f2);
	float FindMaxInVector(XMVECTOR v);
	XMVECTOR VectorConstantLerp(XMVECTOR current, XMVECTOR target, float deltaTime, float interpSpeed);
	XMVECTOR QuatConstantLerp(XMVECTOR current, XMVECTOR target, float deltaTime, float interpSpeed);
	float QuatAngularDistance(XMVECTOR q1, XMVECTOR q2);
	XMVECTOR SmoothStep(XMVECTOR V0, XMVECTOR V1, float t);

	XMMATRIX GetBoundingBoxMatrix(BoundingOrientedBox& boundingBox, Actor* actor);
	void UpdateBoundingBox(BoundingOrientedBox& boundingBox, Actor* actor);
	BoundingOrientedBox GetUpdatedBoundingBox(SpatialComponent* sc);

	float RandomRange(float min, float max);
}
