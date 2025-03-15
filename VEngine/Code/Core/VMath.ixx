export module Core.VMath;

#include <DirectXMath.h>
#include <DirectXCollision.h>

import <unordered_map>;

export class Actor;
export struct Transform;
export class SpatialComponent;
export struct Vertex;

using namespace DirectX;

export namespace VMath
{
	XMVECTOR GlobalRightVector();
	XMVECTOR GlobalUpVector();
	XMVECTOR GlobalForwardVector();

	XMVECTOR RandomUnitDirectionVector();
	XMVECTOR RandomCardinalDirectionVector();

	//Direction vector from quaternion rotation functions (won't take parent transforms into account)
	//For now they're working off of matrices, but below you can extract from the quat directly if needed.
	//Ref:https://www.gamedev.net/forums/topic/56471-extracting-direction-vectors-from-quaternion/
	XMVECTOR ForwardFromQuat(XMVECTOR rot);
	XMVECTOR RightFromQuat(XMVECTOR rot);
	XMVECTOR UpFromQuat(XMVECTOR rot);

	//Returns matrix with all 0's
	XMMATRIX ZeroMatrix();
	void MatrixAddScale(float s, XMMATRIX& m);
	void MatrixAddPosition(XMVECTOR pos, XMMATRIX& m);
	void MatrixAddRotation(XMVECTOR rot, XMMATRIX& m);

	XMFLOAT2 Float2Subtract(XMFLOAT2& f0, XMFLOAT2& f1);
	XMFLOAT3 Float3Subtract(XMFLOAT3& f0, XMFLOAT3& f1);
	bool Float3IsZero(XMFLOAT3& float3);
	void RoundFloat3(XMFLOAT3& float3);
	void CeilFloat3(XMFLOAT3& float3);
	void SaturateFloat4(XMFLOAT4& float4);

	//Good for when XMVECTOR floats are all over the place
	void RoundVector(XMVECTOR& v);

	XMFLOAT3 PitchYawRollFromMatrix(XMMATRIX m);
	void PitchYawRollFromQuaternion(float& roll, float& pitch, float& yaw, XMVECTOR quat);

	//Builds a matrix to extract a quaternion rotation to return.
	XMVECTOR LookAtRotation(XMVECTOR lookAtPoint, XMVECTOR currentPosition);
	XMVECTOR AddRotationAngle(XMVECTOR rotationQuat, XMVECTOR axis, float angleDegrees);

	//Mainly for GridNodes to orient themselves into a rotation based on hit normal via Raycast,
	//giving a nice FFTactics look for the Grid.
	XMMATRIX MakeRotationFromYAxis(XMVECTOR axis);

	//Rotates on the Y-axis
	void RotateTowardsCamera(Transform& transform, float zAngle = 0.f);

	bool VecEqual(XMVECTOR v1, XMVECTOR v2, float epsilon = 9.9999999747e-05f);
	bool FloatEqualRange(float f1, float f2, float epsilon);
	bool Float4Equal(XMFLOAT4& f1, XMFLOAT4& f2);
	bool Float3Equal(XMFLOAT3& f1, XMFLOAT3& f2);
	bool Float2Equal(XMFLOAT2& f1, XMFLOAT2& f2);

	float FindMaxInVector(XMVECTOR v);
	XMVECTOR VectorConstantLerp(XMVECTOR current, XMVECTOR target, float deltaTime, float interpSpeed);
	XMVECTOR QuatConstantLerp(XMVECTOR current, XMVECTOR target, float deltaTime, float interpSpeed);
	float QuatAngularDistance(XMVECTOR q1, XMVECTOR q2);
	XMVECTOR SmoothStep(XMVECTOR V0, XMVECTOR V1, float t);

	XMMATRIX GetBoundingBoxMatrix(BoundingOrientedBox& boundingBox, Actor* actor);
	void UpdateBoundingBox(BoundingOrientedBox& boundingBox, Actor* actor);

	//Note: don't apply translation offset in Blender when exporting and using this,
	//else the bounds calc will be off center.
	BoundingOrientedBox CreateBoundingBox(Vertex* vertices, size_t verticesCount);

	XMVECTOR ConvertToHomogenousPosition(const XMVECTOR positionToConvert);

	float Distance(XMVECTOR pos1, XMVECTOR pos2);

	bool IsToRightOfVector(XMVECTOR v0, XMVECTOR v1);

	//Calculate middle position between two points in-world.
	XMVECTOR CalcMidPoint(XMVECTOR p0, XMVECTOR p1);

	float RandomRange(float min, float max);
	int RandomRangeInt(int min, int max);
	XMFLOAT3 RandomRangeFloat3(XMFLOAT3 v0, XMFLOAT3 v1);
	float InverseLerp(float x, float y, float value);

	//Lightmap and raycast math helper functions
	XMVECTOR TriangleUVToXYZ(XMFLOAT2 uv, Vertex tri[3]);
	bool IsUVInTriangleUVs(XMFLOAT2 pt, XMFLOAT2 v1, XMFLOAT2 v2, XMFLOAT2 v3);
	void GetBarycentricCoords(XMVECTOR p0, XMVECTOR p1, XMVECTOR p2, XMVECTOR hitPoint, float& b1, float& b2);
	void TriangleXYZToUV(Vertex& v0, Vertex& v1, Vertex& v2, XMVECTOR hitPoint, float& u, float& v);
	int GetIndexOfClosestVertexFromTriangleIntersect(std::unordered_map<int, XMVECTOR>& vertexPositions, XMVECTOR hitPoint);

	void HomogenousWorldPosToScreenSpaceCoords(XMVECTOR worldPos, int& screenX, int& screenY);
}
