#pragma once

#include <DirectXMath.h>
#include <vector>
#include <string>
#include <unordered_map>

struct AnimFrame
{
	double time;
	DirectX::XMFLOAT3 pos;

	//TODO: Can propably get away with just using uniform scaling here (Turn XMFLOAt3 to single float)
	//Gotta read up some more, can guarantee shearing would be fucking imposible.
	DirectX::XMFLOAT3 scale;
	DirectX::XMFLOAT4 rot; //Quaternion
};

struct BoneAnimation
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

	std::vector<AnimFrame> frames;
};

struct AnimationClip
{
	float GetStartClipTime();
	float GetEndClipTime();
	void Interpolate(float t, std::vector<DirectX::XMFLOAT4X4>& m);

	//TODO: think about changing anim data like this to std::array.
	//size should be known ahead of time from keyCount.
	std::vector<BoneAnimation> boneAnimations;
};

class SkinnedData
{
public:
	void GetFinalTransforms(const std::string& clipName, float timePos, std::vector<DirectX::XMFLOAT4X4>& finalTransforms);

	uint32_t GetBoneCount()
	{
		return boneOffsets.size();
	}

	std::vector<int> boneHierarchy;
	std::vector<DirectX::XMFLOAT4X4> boneOffsets;
	std::unordered_map<std::string, AnimationClip> animationClips;
};