#include <DirectXMath.h>

export module Animation.AnimFrame;

export struct AnimFrame
{
	DirectX::XMFLOAT4 rot = DirectX::XMFLOAT4(0.f, 0.f, 0.f, 1.f); //Quaternion
	DirectX::XMFLOAT3 pos = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
	DirectX::XMFLOAT3 scale = DirectX::XMFLOAT3(1.f, 1.f, 1.f);
	double time = 0.0;
};
