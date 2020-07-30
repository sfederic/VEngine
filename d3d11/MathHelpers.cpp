#include "MathHelpers.h"

void MatrixAddScale(float s, XMMATRIX& m)
{
    m.r[0].m128_f32[0] = s;
    m.r[1].m128_f32[1] = s;
    m.r[2].m128_f32[2] = s;
}

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
