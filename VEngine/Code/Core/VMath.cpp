#include "vpch.h"
#include "VMath.h"
#include <cmath>
#include <algorithm>
#include <array>
#include <random>
#include "Actors/Actor.h"
#include "Camera.h"
#include "Render/Vertex.h"
#include "Render/Renderer.h"

namespace VMath
{
	void MatrixAddScale(float s, XMMATRIX& m)
	{
		m.r[0].m128_f32[0] = s;
		m.r[1].m128_f32[1] = s;
		m.r[2].m128_f32[2] = s;
	}

	void MatrixAddPosition(XMVECTOR pos, XMMATRIX& m)
	{
		m.r[3] += pos;
	}

	void MatrixAddRotation(XMVECTOR rot, XMMATRIX& m)
	{
		m *= XMMatrixRotationQuaternion(rot);
	}

	//GLOBAL VECTOR DIRECTIONS (X+ = Right, Y+ = Up, Z+ = forward)
	XMVECTOR GlobalRightVector()
	{
		return XMVectorSet(1.f, 0.f, 0.f, 0.f);
	}

	XMVECTOR GlobalUpVector()
	{
		return XMVectorSet(0.f, 1.f, 0.f, 0.f);
	}

	XMVECTOR GlobalForwardVector()
	{
		return XMVectorSet(0.f, 0.f, 1.f, 0.f);
	}

	XMVECTOR RandomUnitDirectionVector()
	{
		XMFLOAT3 minFloat3 = XMFLOAT3(-1.f, -1.f, -1.f);
		XMFLOAT3 maxFloat3 = XMFLOAT3(1.f, 1.f, 1.f);
		XMFLOAT3 rand = VMath::RandomRangeFloat3(minFloat3, maxFloat3);
		XMVECTOR randV = XMLoadFloat3(&rand);
		randV = XMVector3Normalize(randV);
		return randV;
	}

	XMVECTOR RandomCardinalDirectionVector()
	{
		std::array<XMVECTOR, 6> directions = {
			VMath::GlobalRightVector(),
			-VMath::GlobalRightVector(),
			VMath::GlobalUpVector(),
			-VMath::GlobalUpVector(),
			VMath::GlobalForwardVector(),
			-VMath::GlobalForwardVector(),
		};

		const size_t randomDirectionIndex = VMath::RandomRangeInt(0, directions.size() - 1);
		XMVECTOR selectedDirection = directions[randomDirectionIndex];
		return selectedDirection;
	}

	//Read over over from Unreal (UKismetMathLibrary::MakeRotFromZ)
	XMMATRIX MakeRotationFromYAxis(XMVECTOR yAxis)
	{
		yAxis = XMVector3Normalize(yAxis);

		constexpr float SMALL_NUMBER = 1.e-4f;
		const XMVECTOR forwardVector = (std::abs(yAxis.m128_f32[2]) < (1.f - SMALL_NUMBER)) ?
			VMath::GlobalForwardVector() : VMath::GlobalRightVector();

		const XMVECTOR xAxis = XMVector3Normalize(XMVector3Cross(forwardVector, yAxis));
		const XMVECTOR zAxis = XMVector3Cross(yAxis, xAxis);

		//Reverse the X-Axis here to fit with DirectX's coordinate system.
		return XMMATRIX(-xAxis, yAxis, zAxis, XMVectorZero());
	}

	XMMATRIX ZeroMatrix()
	{
		return XMMATRIX();
	}

	bool Float3IsZero(XMFLOAT3& float3)
	{
		return float3.x == 0.f && float3.y == 0.f && float3.z == 0.f;
	}

	void RoundFloat3(XMFLOAT3& float3)
	{
		float3.x = std::round(float3.x);
		float3.y = std::round(float3.y);
		float3.z = std::round(float3.z);
	}

	void CeilFloat3(XMFLOAT3& float3)
	{
		float3.x = std::ceil(float3.x);
		float3.y = std::ceil(float3.y);
		float3.z = std::ceil(float3.z);
	}

	void SaturateFloat4(XMFLOAT4& float4)
	{
		float4.x = std::clamp(float4.x, 0.f, 1.0f);
		float4.y = std::clamp(float4.y, 0.f, 1.0f);
		float4.z = std::clamp(float4.z, 0.f, 1.0f);
		float4.w = std::clamp(float4.w, 0.f, 1.0f);
	}

	void RoundVector(XMVECTOR& v)
	{
		v.m128_f32[0] = std::roundf(v.m128_f32[0]);
		v.m128_f32[1] = std::roundf(v.m128_f32[1]);
		v.m128_f32[2] = std::roundf(v.m128_f32[2]);
		v.m128_f32[3] = std::roundf(v.m128_f32[3]);
	}

	XMVECTOR XMVectorConstantLerp(FXMVECTOR V0, FXMVECTOR V1, float deltaTime, float speed)
	{
		XMVECTOR v = XMVectorLerp(V0, V1, deltaTime * speed);
		return v;
	}

	//REF:https://docs.microsoft.com/en-us/windows/win32/api/directxmath/nf-directxmath-xmvectorlerp
	XMVECTOR SmoothStep(XMVECTOR V0, XMVECTOR V1, float t)
	{
		t = (t > 1.0f) ? 1.0f : ((t < 0.0f) ? 0.0f : t);  // Clamp value to 0 to 1
		t = t * t * (3.f - 2.f * t);
		return XMVectorLerp(V0, V1, t);
	}

	XMFLOAT3 PitchYawRollFromMatrix(XMMATRIX m)
	{
		//REF: https://en.wikipedia.org/wiki/Rotation_formalisms_in_three_dimensions#Conversion_formulae_between_formalisms
		float pitch = acosf(m.r[2].m128_f32[2]);
		float yaw = atan2f(m.r[0].m128_f32[2], m.r[1].m128_f32[2]);
		float roll = atan2f(m.r[2].m128_f32[0], m.r[2].m128_f32[1]);

		pitch = XMConvertToDegrees(pitch);
		yaw = XMConvertToDegrees(yaw);
		roll = XMConvertToDegrees(roll);

		return XMFLOAT3(pitch, yaw, roll);
	}

	//Finding this was the fucking worst. All sorts of wrong mathematical answers on established sites
	//where answers/proofs were mistaking roll for pitch, converting to degrees in radian operations,
	//incorrect asin/atan2 operations. What a fucking mess. In the end, a Unity answer was correct.
	//Ref:https://answers.unity.com/questions/416169/finding-pitchrollyaw-from-quaternions.html
	void PitchYawRollFromQuaternion(float& roll, float& pitch, float& yaw, XMVECTOR quat)
	{
		XMFLOAT4 q{};
		XMStoreFloat4(&q, quat);
		roll = std::atan2(2 * q.y * q.w - 2 * q.x * q.z, 1 - 2 * q.y * q.y - 2 * q.z * q.z);
		pitch = std::atan2(2 * q.x * q.w - 2 * q.y * q.z, 1 - 2 * q.x * q.x - 2 * q.z * q.z);
		yaw = std::asin(2 * q.x * q.y + 2 * q.z * q.w);
	}

	XMVECTOR LookAtRotation(XMVECTOR lookAtPoint, XMVECTOR currentPosition)
	{
		XMVECTOR forward = XMVectorSubtract(lookAtPoint, currentPosition);
		forward = XMVector3Normalize(forward);

		XMVECTOR right = XMVector3Cross(GlobalUpVector(), forward);
		right = XMVector3Normalize(right);

		XMVECTOR up = XMVector3Normalize(XMVector3Cross(forward, right));
		XMMATRIX m = {};
		m.r[0] = right;
		m.r[1] = up;
		m.r[2] = forward;
		m.r[3] = currentPosition;

		XMVECTOR Q = XMQuaternionRotationMatrix(m);
		return Q;
	}

	XMVECTOR AddRotationAngle(XMVECTOR rotationQuat, XMVECTOR axis, float angleDegrees)
	{
		return XMQuaternionMultiply(rotationQuat, XMQuaternionRotationAxis(axis, XMConvertToRadians(angleDegrees)));
	}

	XMVECTOR ForwardFromQuat(XMVECTOR rot)
	{
		auto m = XMMatrixRotationQuaternion(rot);
		auto f = XMVector3Normalize(m.r[2]);
		return f;
	}

	XMVECTOR RightFromQuat(XMVECTOR rot)
	{
		auto m = XMMatrixRotationQuaternion(rot);
		auto r = XMVector3Normalize(m.r[0]);
		return r;
	}

	XMVECTOR UpFromQuat(XMVECTOR rot)
	{
		auto m = XMMatrixRotationQuaternion(rot);
		auto u = XMVector3Normalize(m.r[1]);
		return u;
	}

	void RotateTowardsCamera(Transform& transform, float zAngle)
	{
		const float posX = transform.position.x;
		const float posZ = transform.position.z;

		//Make sure to use the world matrix here because some camera (Player camera) will 
		//have the camera at an offset from its parent.
		const XMMATRIX cameraWorldMatrix = Camera::GetActiveCamera().GetWorldMatrix();

		const float angle = atan2(cameraWorldMatrix.r[3].m128_f32[0] - posX,
			cameraWorldMatrix.r[3].m128_f32[2] - posZ) * (180.0 / XM_PI);

		const float rotation = XMConvertToRadians(angle);

		XMMATRIX m = XMMatrixRotationY(rotation);
		if (zAngle != 0.f)
		{
			//Rotate on the "Z-axis" facing towards camera
			m *= XMMatrixRotationAxis(m.r[2], XMConvertToRadians(zAngle));
		}
		const XMVECTOR rot = XMQuaternionRotationMatrix(m);

		//Set rotation
		XMStoreFloat4(&transform.rotation, rot);
	}

	bool VecEqual(XMVECTOR v1, XMVECTOR v2, float epsilon)
	{
		return fabsf(v1.m128_f32[0] - v2.m128_f32[0]) <= epsilon &&
			fabsf(v1.m128_f32[1] - v2.m128_f32[1]) <= epsilon &&
			fabsf(v1.m128_f32[2] - v2.m128_f32[2]) <= epsilon;
	}

	bool FloatEqualRange(float f1, float f2, float epsilon)
	{
		return fabsf(f1 - f2) <= epsilon;
	}

	bool Float4Equal(XMFLOAT4& f1, XMFLOAT4& f2)
	{
		if (f1.x == f2.x && f1.y == f2.y && f1.z == f2.z && f1.w == f2.w)
		{
			return true;
		}

		return false;
	}

	bool Float3Equal(XMFLOAT3& f1, XMFLOAT3& f2)
	{
		if (f1.x == f2.x && f1.y == f2.y && f1.z == f2.z)
		{
			return true;
		}

		return false;
	}

	bool Float2Equal(XMFLOAT2& f1, XMFLOAT2& f2)
	{
		if (f1.x == f2.x && f1.y == f2.y)
		{
			return true;
		}

		return false;
	}

	float FindMaxInVector(XMVECTOR v)
	{
		float max = 0.f;
		for (int i = 0; i < 4; i++)
		{
			if (max < v.m128_f32[i])
			{
				max = v.m128_f32[i];
			}
		}

		return max;
	}

	//Read over from Unreal
	XMVECTOR VectorConstantLerp(XMVECTOR current, XMVECTOR target, float deltaTime, float interpSpeed)
	{
		XMVECTOR toTarget = XMVectorSubtract(target, current);

		const float magnitude = XMVector3Length(toTarget).m128_f32[0];
		const float step = deltaTime * interpSpeed;

		if (magnitude > step)
		{
			if (step > 0.0f)
			{
				const XMVECTOR diff = toTarget / magnitude;
				return current + diff * step;
			}
			else
			{
				return current;
			}
		}

		return target;
	}

	//Read over from Unreal
	XMVECTOR QuatConstantLerp(XMVECTOR current, XMVECTOR target, float deltaTime, float interpSpeed)
	{
		float deltaInterpSpeed = std::clamp(deltaTime * interpSpeed, 0.f, 1.f);
		float AngularDistance = std::max(1.e-8f, QuatAngularDistance(target, current));
		float alpha = std::clamp(deltaInterpSpeed / AngularDistance, 0.f, 1.f);

		return XMQuaternionSlerp(current, target, alpha);
	}

	float QuatAngularDistance(XMVECTOR q1, XMVECTOR q2)
	{
		float innerProduct = q1.m128_f32[0] * q2.m128_f32[0] +
			q1.m128_f32[1] * q2.m128_f32[1] +
			q1.m128_f32[2] * q2.m128_f32[2] +
			q1.m128_f32[3] * q2.m128_f32[3];
		return acosf((2 * innerProduct * innerProduct) - 1.f);
	}

	XMMATRIX GetBoundingBoxMatrix(BoundingOrientedBox& boundingBox, Actor* actor)
	{
		UpdateBoundingBox(boundingBox, actor);

		XMFLOAT3 extents = XMFLOAT3(boundingBox.Extents.x * 2.f, boundingBox.Extents.y * 2.f,
			boundingBox.Extents.z * 2.f);
		XMMATRIX boxBoundsMatrix = XMMatrixAffineTransformation(XMLoadFloat3(&extents),
			XMVectorSet(0.f, 0.f, 0.f, 1.f),
			XMLoadFloat4(&boundingBox.Orientation),
			XMLoadFloat3(&boundingBox.Center));

		return boxBoundsMatrix;
	}

	void UpdateBoundingBox(BoundingOrientedBox& boundingBox, Actor* actor)
	{
		XMVECTOR actorPos = actor->GetPositionV();
		XMVECTOR boundingBoxCenter = XMLoadFloat3(&boundingBox.Center);
		XMVECTOR offset = actorPos + boundingBoxCenter;
		offset.m128_f32[3] = 1.0f;

		XMVECTOR actorScale = actor->GetScaleV();
		XMVECTOR extents = XMLoadFloat3(&boundingBox.Extents);
		XMVECTOR scale = extents * actorScale;
		scale.m128_f32[3] = 1.0f;

		XMVECTOR orientation = actor->GetRotationV();

		XMStoreFloat3(&boundingBox.Center, offset);
		XMStoreFloat3(&boundingBox.Extents, scale);
		XMStoreFloat4(&boundingBox.Orientation, orientation);
	}

	BoundingOrientedBox CreateBoundingBox(Vertex* vertices, size_t verticesCount)
	{
		assert(verticesCount > 0);

		float minX = vertices[0].pos.x;
		float minY = vertices[0].pos.y;
		float minZ = vertices[0].pos.z;

		float maxX = minX;
		float maxY = minY;
		float maxZ = minZ;

		for (size_t i = 0; i < verticesCount; i++)
		{
			float x = vertices[i].pos.x;
			float y = vertices[i].pos.y;
			float z = vertices[i].pos.z;

			if (x > maxX) maxX = x;
			if (x < minX) minX = x;

			if (y > maxY) maxY = y;
			if (y < minY) minY = y;

			if (z > maxZ) maxZ = z;
			if (z < minZ) minZ = z;
		}

		//Remember that extents are halved
		return BoundingOrientedBox(XMFLOAT3(0.f, 0.f, 0.f),
			XMFLOAT3((maxX - minX) / 2.f, (maxY - minY) / 2.f, (maxZ - minZ) / 2.f),
			XMFLOAT4(0.f, 0.f, 0.f, 1.f));
	}

	XMVECTOR ConvertToHomogenousPosition(const XMVECTOR positionToConvert)
	{
		auto& activeCamera = Camera::GetActiveCamera();
		const XMMATRIX view = activeCamera.GetViewMatrix();
		const XMMATRIX proj = activeCamera.GetProjectionMatrix();
		const XMMATRIX worldMatrix = XMMatrixTranslationFromVector(positionToConvert);
		const XMMATRIX mvp = worldMatrix * view * proj;
		return mvp.r[3];
	}

	float Distance(XMVECTOR pos1, XMVECTOR pos2)
	{
		return XMVector3Length(pos1 - pos2).m128_f32[0];
	}

	bool IsToRightOfVector(XMVECTOR v0, XMVECTOR v1)
	{
		const auto delta = XMVector3Normalize(v0);
		const auto cross = XMVector3Cross(delta, v1);

		if (cross.m128_f32[1] > 0.f) //vector is to the right
		{
			return true;
		}
		else //vector is to the left
		{
			return false;
		}
	}

	XMVECTOR CalcMidPoint(XMVECTOR p0, XMVECTOR p1)
	{
		return (p0 + p1) / 2;
	}

	float RandomRange(float min, float max)
	{
		if (max < min)
		{
			return min;
		}

		if (min > max)
		{
			return max;
		}

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution dist(min, max);
		float result = dist(gen);
		return result;
	}

	int RandomRangeInt(int min, int max)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution dist(min, max);
		int result = dist(gen);
		return result;
	}

	XMFLOAT3 RandomRangeFloat3(XMFLOAT3 minFloat3, XMFLOAT3 maxFloat3)
	{
		XMFLOAT3 f3{};
		f3.x = VMath::RandomRange(minFloat3.x, maxFloat3.x);
		f3.y = VMath::RandomRange(minFloat3.y, maxFloat3.y);
		f3.z = VMath::RandomRange(minFloat3.z, maxFloat3.z);
		return f3;
	}

	float InverseLerp(float x, float y, float value)
	{
		return (value - x) / (y - x);
	}

	XMFLOAT3 Float3Subtract(XMFLOAT3& f0, XMFLOAT3& f1)
	{
		return XMFLOAT3(f0.x - f1.x, f0.y - f1.y, f0.z - f1.z);
	}

	XMFLOAT2 Float2Subtract(XMFLOAT2& f0, XMFLOAT2& f1)
	{
		return XMFLOAT2(f0.x - f1.x, f0.y - f1.y);
	}

	//Ref:https://stackoverflow.com/questions/72209091/how-to-given-uv-on-a-triangle-find-xyz
	XMVECTOR TriangleUVToXYZ(XMFLOAT2 uv, Vertex tri[3])
	{
		XMVECTOR tri12 = XMLoadFloat2(&tri[1].uv) - XMLoadFloat2(&tri[0].uv);
		XMVECTOR tri13 = XMLoadFloat2(&tri[2].uv) - XMLoadFloat2(&tri[0].uv);
		XMVECTOR p1 = XMLoadFloat2(&uv) - XMLoadFloat2(&tri[0].uv);

		XMVECTOR a23 = XMVector2Cross(tri12, tri13);

		XMVECTOR a12 = XMVector2Cross(p1, tri12) / -a23;
		XMVECTOR a13 = XMVector2Cross(p1, tri13) / a23;

		return XMLoadFloat3(&tri[0].pos)
			+ a12 * (XMLoadFloat3(&tri[1].pos) - XMLoadFloat3(&tri[0].pos))
			+ a13 * (XMLoadFloat3(&tri[2].pos) - XMLoadFloat3(&tri[0].pos));
	}

	//Ref:https://stackoverflow.com/questions/2049582/how-to-determine-if-a-point-is-in-a-2d-triangle
	bool IsUVInTriangleUVs(XMFLOAT2 pt, XMFLOAT2 v1, XMFLOAT2 v2, XMFLOAT2 v3)
	{
		auto sign = [](const XMFLOAT2& p1, const XMFLOAT2& p2, const XMFLOAT2& p3)
			{
				return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
			};

		float d1 = sign(pt, v1, v2);
		float d2 = sign(pt, v2, v3);
		float d3 = sign(pt, v3, v1);

		bool hasNeg = (d1 < 0) || (d2 < 0) || (d3 < 0);
		bool hasPos = (d1 > 0) || (d2 > 0) || (d3 > 0);

		return !(hasNeg && hasPos);
	}

	//Ref:https://stackoverflow.com/questions/17164376/inferring-u-v-for-a-point-in-a-triangle-from-vertex-u-vs
	void GetBarycentricCoords(XMVECTOR p0, XMVECTOR p1, XMVECTOR p2, XMVECTOR hitPoint, float& b1, float& b2)
	{
		const XMVECTOR u = p1 - p0;
		const XMVECTOR v = p2 - p0;
		const XMVECTOR w = hitPoint - p0;

		const XMVECTOR vCrossW = XMVector3Cross(v, w);
		const XMVECTOR vCrossU = XMVector3Cross(v, u);

		const XMVECTOR uCrossW = XMVector3Cross(u, w);
		const XMVECTOR uCrossV = XMVector3Cross(u, v);

		const float denom = XMVector3Length(uCrossV).m128_f32[0];
		const float r = XMVector3Length(vCrossW).m128_f32[0] / denom;
		const float t = XMVector3Length(uCrossW).m128_f32[0] / denom;

		b1 = r;
		b2 = t;
	}

	void TriangleXYZToUV(Vertex& v0, Vertex& v1, Vertex& v2, XMVECTOR hitPoint, float& u, float& v)
	{
		XMVECTOR p0 = XMLoadFloat3(&v0.pos);
		XMVECTOR p1 = XMLoadFloat3(&v1.pos);
		XMVECTOR p2 = XMLoadFloat3(&v2.pos);

		float b1, b2;
		GetBarycentricCoords(p0, p1, p2, hitPoint, b1, b2);

		float u_v0 = v0.uv.x;
		float u_v1 = v1.uv.x;
		float u_v2 = v2.uv.x;

		float v_v0 = v0.uv.y;
		float v_v1 = v1.uv.y;
		float v_v2 = v2.uv.y;

		const float b0 = 1.f - b1 - b2;
		u = b0 * u_v0 + b1 * u_v1 + b2 * u_v2;
		v = b0 * v_v0 + b1 * v_v1 + b2 * v_v2;
	}

	//Make sure to put the vertex positions into world space
	int GetIndexOfClosestVertexFromTriangleIntersect(std::unordered_map<int, XMVECTOR>& vertexPositions, XMVECTOR hitPoint)
	{
		int vertexIndex = -1;
		float lowestDistance = std::numeric_limits<float>::max();
		for (const auto& [index, position] : vertexPositions)
		{
			const float distance = XMVector3Length(position - hitPoint).m128_f32[0];
			if (distance < lowestDistance)
			{
				lowestDistance = distance;
				vertexIndex = index;
			}
		}

		assert(vertexIndex != -1);
		return vertexIndex;
	}

	void HomogenousWorldPosToScreenSpaceCoords(XMVECTOR worldPos, int& screenX, int& screenY)
	{
		//What you need to do here it take the actor's position after it's been multiplied 
		//by the MVP matrix on the CPU side of things (Actor::GetHomogenousPositionV(),
		//divide it by the W component and multiply it out by the viewport.
		//REF:http://www.windows-tech.info/5/a80747e145dd9062.php

		const float f1 = worldPos.m128_f32[0] / worldPos.m128_f32[3];
		const float f2 = worldPos.m128_f32[1] / worldPos.m128_f32[3];

		screenX = ((f1 * 0.5f) + 0.5) * Renderer::GetViewportWidth();
		screenY = ((f2 * -0.5f) + 0.5) * Renderer::GetViewportHeight();
	}
}
