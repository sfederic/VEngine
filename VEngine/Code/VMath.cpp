#include "VMath.h"
#include <cmath>
#include <algorithm>
#include "Actors/Actor.h"

namespace VMath
{
    void MatrixAddScale(float s, XMMATRIX& m)
    {
        m.r[0].m128_f32[0] = s;
        m.r[1].m128_f32[1] = s;
        m.r[2].m128_f32[2] = s;
    }

    //GLOBAL VECTOR DIRECTIONS (X+ = Right, Y+ = Up, Z+ = forward)
    XMVECTOR XMVectorRight()
    {
        return XMVectorSet(1.f, 0.f, 0.f, 0.f);
    }

    XMVECTOR XMVectorUp()
    {
        return XMVectorSet(0.f, 1.f, 0.f, 0.f);
    }

    XMVECTOR XMVectorForward()
    {
        return XMVectorSet(0.f, 0.f, 1.f, 0.f);
    }

    void RoundFloat3(XMFLOAT3& float3)
    {
        float3.x = std::round(float3.x);
        float3.y = std::round(float3.y);
        float3.z = std::round(float3.z);
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

    XMFLOAT3 PitchYawRollFromQuaternion(XMFLOAT4 q)
    {
        XMMATRIX m = XMMatrixRotationQuaternion(XMLoadFloat4(&q));
        return XMFLOAT3(PitchYawRollFromMatrix(m));
    }

    void LookAtRotation(XMVECTOR lookAtPoint, XMMATRIX& m)
    {
        //lookAtPoint.m128_f32[3] = 0.f;
        XMVECTOR forward = XMVectorAdd(lookAtPoint, m.r[3]);
        forward = XMVector3Normalize(forward);

        XMVECTOR right = XMVector3Cross(XMVectorUp(), forward);
        right = XMVector3Normalize(right);

        XMVECTOR up = XMVector3Cross(forward, right); //Already normalised.
        m.r[0] = right;
        m.r[1] = up;
        m.r[2] = forward;
    }

    XMFLOAT4X4 FbxMatrixToDirectXMathMatrix(fbxsdk::FbxMatrix fbxMatrix)
    {
        //TODO: Fbx likes to work in units of 100. Either do the division or make like UE4.
        return XMFLOAT4X4(fbxMatrix.mData[0].mData[0] / 100.0, fbxMatrix.mData[0].mData[1] / 100.0, fbxMatrix.mData[0].mData[2] / 100.0, fbxMatrix.mData[0].mData[3] / 100.0,
            fbxMatrix.mData[1].mData[0] / 100.0, fbxMatrix.mData[1].mData[1] / 100.0, fbxMatrix.mData[1].mData[2] / 100.0, fbxMatrix.mData[1].mData[3] / 100.0,
            fbxMatrix.mData[2].mData[0] / 100.0, fbxMatrix.mData[2].mData[1] / 100.0, fbxMatrix.mData[2].mData[2] / 100.0, fbxMatrix.mData[2].mData[3] / 100.0,
            fbxMatrix.mData[3].mData[0] / 100.0, fbxMatrix.mData[3].mData[1] / 100.0, fbxMatrix.mData[3].mData[2] / 100.0, fbxMatrix.mData[3].mData[3] / 100.0);
    }

    bool VecEqual(XMVECTOR v1, XMVECTOR v2, float epsilon)
    {
        if (v1.m128_f32[0] <= (v2.m128_f32[0] + epsilon) && v1.m128_f32[0] >= (v2.m128_f32[0] - epsilon) &&
            v1.m128_f32[1] <= (v2.m128_f32[1] + epsilon) && v1.m128_f32[1] >= (v2.m128_f32[1] - epsilon) &&
            v1.m128_f32[2] <= (v2.m128_f32[2] + epsilon) && v1.m128_f32[2] >= (v2.m128_f32[2] - epsilon) &&
            v1.m128_f32[3] <= (v2.m128_f32[3] + epsilon) && v1.m128_f32[3] >= (v2.m128_f32[3] - epsilon))
        {
            return true;
        }

        return false;
    }

    //bool VecEqual(XMVECTOR v1, XMVECTOR v2, float epsilon)
    //{
    //    XMVECTOR v = XMVectorZero();
    //    v = DirectX::XMVectorEqual(v1, v2);
    //    if (v.m128_f32[0] > 0)
    //    {
    //        return true;
    //    }

    //    return false;
    //}

    bool Float3Equal(XMFLOAT3& f1, XMFLOAT3& f2)
    {
        if (f1.x == f2.x && f1.y == f2.y && f1.z == f2.z)
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

    //Copied heavily from UE4
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

    //Copied heavily from UE4
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
        //XMFLOAT3 actorPosFloat3 = actor->GetPositionFloat3();
        //XMVECTOR actorPos = XMLoadFloat3(&actorPosFloat3);
        //XMVECTOR boundingBoxCenter = XMLoadFloat3(&boundingBox.Center);
        //XMVECTOR offset = actorPos + boundingBoxCenter;
        //offset.m128_f32[3] = 1.0f;

        //XMFLOAT3 actorScaleFloat3 = actor->GetScale();
        //XMVECTOR actorScale = XMLoadFloat3(&actorScaleFloat3);
        //XMVECTOR extents = XMLoadFloat3(&boundingBox.Extents);
        //extents.m128_f32[3] = 1.0f;

        XMMATRIX boxBoundsMatrix = XMMatrixIdentity();
        //boxBoundsMatrix = actor->GetTransformationMatrix(); //This is more just to get the rotation
        //boxBoundsMatrix *= XMMatrixScalingFromVector(extents);
        //boxBoundsMatrix.r[3] = offset;

        return boxBoundsMatrix;
    }

    void UpdateBoundingBox(BoundingOrientedBox& boundingBox, Actor* actor)
    {
        //XMFLOAT3 actorPosFloat3 = actor->GetPositionFloat3();
        //XMVECTOR actorPos = XMLoadFloat3(&actorPosFloat3);
        //XMVECTOR boundingBoxCenter = XMLoadFloat3(&boundingBox.Center);
        //XMVECTOR offset = actorPos + boundingBoxCenter;
        //offset.m128_f32[3] = 1.0f;

        //XMFLOAT3 actorScaleFloat3 = actor->GetScale();
        //XMVECTOR actorScale = XMLoadFloat3(&actorScaleFloat3);
        //XMVECTOR extents = XMLoadFloat3(&boundingBox.Extents);
        //XMVECTOR scale = extents * actorScale;
        //scale.m128_f32[3] = 1.0f;

        //XMFLOAT4 actorRotFloat4 = actor->GetRotationQuat();
        //XMVECTOR orientation = XMLoadFloat4(&actorRotFloat4);

        //XMStoreFloat3(&boundingBox.Center, offset);
        //XMStoreFloat3(&boundingBox.Extents, scale);
        //XMStoreFloat4(&boundingBox.Orientation, orientation);
    }
}
