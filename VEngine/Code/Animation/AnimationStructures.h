#pragma once
#include <DirectXMath.h>
#include <vector>
#include <string>
#include <array>

using namespace DirectX;

struct Skeleton;
struct Joint;

struct BoneWeights
{
	inline static int MAX_WEIGHTS = 3;
	inline static int MAX_BONE_INDICES = 4;

	//@Todo: these really should be an array, but it's a pain in the ass to fill them up in the FBXImporter
	std::vector<double> weights;
	std::vector<int> boneIndex;
};

struct AnimFrame
{
	double time;
	XMFLOAT4 rot; //Quaternion
	XMFLOAT3 pos;
	XMFLOAT3 scale;
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

	void Interpolate(float t, Joint& joint, Skeleton* skeleton);

	std::string name;

	std::vector<AnimFrame> frames;

	float currentTime = 0.f;
};


struct Joint
{
	std::string name;
	int parentIndex = -1;
	int index = 0;
	XMMATRIX invBindPose = XMMatrixIdentity();

	Animation anim;
};

struct Skeleton
{
	std::vector<Joint> joints;

	void AddJoint(Joint joint);
	int FindJointIndexByName(std::string name);
};
