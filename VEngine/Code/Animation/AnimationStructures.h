#pragma once
#include <DirectXMath.h>
#include <vector>
#include <string>

using namespace DirectX;

struct AnimFrame
{
	double time;
	XMFLOAT3 pos;
	XMFLOAT3 scale;
	XMFLOAT4 rot; //Quaternion
};

struct Animation
{
	float GetStartTime()
	{
		return frames.front().time;
	}

	float GetEndTime()
	{
		return frames.back().time;
	}

	void Interpolate(float t, DirectX::XMFLOAT4X4& m);

	std::string name;

	std::vector<AnimFrame> frames;
};
