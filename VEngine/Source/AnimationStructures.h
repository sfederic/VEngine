#pragma once

#include <DirectXMath.h>
#include <vector>
#include <string>
#include <unordered_map>

struct AnimFrame
{
	double time;
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 scale;
	DirectX::XMFLOAT4 rot; //Quaternion
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
