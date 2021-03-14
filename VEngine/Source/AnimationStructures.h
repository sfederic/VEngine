#pragma once

#include <DirectXMath.h>
#include <vector>

struct AnimFrame
{
	double time;
	DirectX::XMFLOAT3 pos;

	//TODO: Can propably get away with just using uniform scaling here (Turn XMFLOAt3 to single float)
	//Gotta read up some more, can guarantee shearing would be fucking imposible.
	DirectX::XMFLOAT3 scale;
	DirectX::XMFLOAT4 rot; //Quat
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

	//TODO: think about changing anim data like this to std::array.
	//size should be known ahead of time from keyCount.
	std::vector<AnimFrame> frames;
};