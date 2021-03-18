#include "MathHelpers.h"
#include <math.h>

void MatrixAddScale(float s, XMMATRIX& m)
{
    m.r[0].m128_f32[0] = s;
    m.r[1].m128_f32[1] = s;
    m.r[2].m128_f32[2] = s;
}

//GLOBAL VECTOR DIRECTIONS
XMVECTOR XMVectorRight()
{
    __m128 vec = _mm_set_ps(0.f, 0.f, 0.f, 1.f);
    return vec;
}

XMVECTOR XMVectorUp()
{
    __m128 vec = _mm_set_ps(0.f, 0.f, 1.f, 0.f);
    return vec;
}

XMVECTOR XMVectorForward()
{
    __m128 vec = _mm_set_ps(0.f, 1.f, 0.f, 0.f);
    return vec;
}

/*XMVECTOR XMVectorConstantLerp(FXMVECTOR V0, FXMVECTOR V1, float deltaTime, float speed)
{
    XMVECTOR v = XMVectorLerp(V0, V1, deltaTime);
    return v * speed;
}*/

XMVECTOR RollPitchYawFromMatrix(XMMATRIX m)
{
    //REF: https://en.wikipedia.org/wiki/Rotation_formalisms_in_three_dimensions#Conversion_formulae_between_formalisms
    float roll = atan2f(m.r[2].m128_f32[0], m.r[2].m128_f32[1]);
    float pitch = acosf(m.r[2].m128_f32[2]);
    float yaw = atan2f(m.r[0].m128_f32[2], m.r[1].m128_f32[2]);

    XMVECTOR vec = XMVectorSet(roll, pitch, yaw, 0.f);

    return vec;
}

//Direction will usually be your forward vector for an actor/billboard.
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
