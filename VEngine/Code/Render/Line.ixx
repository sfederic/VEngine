export module Render.Line;

#include <DirectXMath.h>

export struct Line
{
	DirectX::XMFLOAT3 p1 = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
	DirectX::XMFLOAT3 p2 = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
};
