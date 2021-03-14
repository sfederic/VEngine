#pragma once

#include <DirectXMath.h>
#include <vector>

struct AnimFrame
{
	double time;
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 scale;
	DirectX::XMFLOAT4 rot; //Quat
};

struct Animation
{
	float GetStartTime();
	float GetEndTime();
	void Interpolate(float t, DirectX::XMFLOAT4X4& m);

	std::vector<AnimFrame> frames;
};