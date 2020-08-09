#include "MathHelpers.h"

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
