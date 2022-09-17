#pragma once
#include <DirectXMath.h>
#include <DirectXCollision.h>

class Actor;
struct Transform;
struct SpatialComponent;
struct Vertex;

using namespace DirectX;

namespace VMath
{
	void MatrixAddScale(float s, XMMATRIX& m);

	XMVECTOR GlobalRightVector();
	XMVECTOR GlobalUpVector();
	XMVECTOR GlobalForwardVector();

	//Returns matrix with all 0's
	XMMATRIX ZeroMatrix();

	void RoundFloat3(XMFLOAT3& float3);
	void RoundVector(XMVECTOR& v);

	XMFLOAT3 PitchYawRollFromMatrix(XMMATRIX m);
	void PitchYawRollFromQuaternion(float& roll, float& pitch, float& yaw, XMVECTOR quat);

	//Builds a matrix to extract a quaternion rotation to return.
	XMVECTOR LookAtRotation(XMVECTOR lookAtPoint, XMVECTOR currentPosition);

	//Direction vector from quaternion rotation functions (won't take parent transforms into account)
	//For now they're working off of matrices, but below you can extract from the quat directly if needed.
	//Ref:https://www.gamedev.net/forums/topic/56471-extracting-direction-vectors-from-quaternion/
	XMVECTOR ForwardFromQuat(XMVECTOR rot);
	XMVECTOR RightFromQuat(XMVECTOR rot);
	XMVECTOR UpFromQuat(XMVECTOR rot);

	//Rotates on the Y-axis
	void RotateTowardsCamera(Transform& transform, float zAngle = 0.f);

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

	//Note: don't apply translation offset in Blender when exporting and using this,
	//else the bounds calc will be off center.
	BoundingOrientedBox CreateBoundingBox(Vertex* vertices, size_t verticesCount);

	float Distance(XMVECTOR pos1, XMVECTOR pos2);

	float RandomRange(float min, float max);

	XMFLOAT3 Float3Subtract(XMFLOAT3& f0, XMFLOAT3& f1);
	XMFLOAT2 Float2Subtract(XMFLOAT2& f0, XMFLOAT2& f1);
}
