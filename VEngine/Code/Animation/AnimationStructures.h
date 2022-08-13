#pragma once
#include <vector>
#include <map>
#include <DirectXMath.h>

using namespace DirectX;

struct Skeleton;
struct Joint;

struct BoneWeights
{
	inline static int MAX_WEIGHTS = 3;
	inline static int MAX_BONE_INDICES = 4;

	std::vector<double> weights;
	std::vector<int> boneIndex;
};

struct AnimFrame
{
	XMFLOAT4 rot; //Quaternion
	XMFLOAT3 pos;
	XMFLOAT3 scale;
	double time;
};

struct Animation
{
	bool isPlaying = true;

	//Maps joint index to AnimFrames
	std::map<int, std::vector<AnimFrame>> frames;

	float GetStartTime(int jointIndex)
	{
		return frames[jointIndex].front().time;
	}

	float GetEndTime(int jointIndex)
	{
		return frames[jointIndex].back().time;
	}

	float GetFinalTime();

	void Interpolate(float t, Joint& joint, Skeleton* skeleton);
};

struct Joint
{
	void SetName(const char* name_)
	{
		strcpy_s(name, sizeof(name), name_);
	}

	XMMATRIX currentPose = XMMatrixIdentity();
	XMMATRIX inverseBindPose = XMMatrixIdentity();

	char name[32];

	int parentIndex = -1; //-1 is the root joint's index or if the bone doesn't have a parent
	int index = 0;
};

struct Skeleton
{
	std::vector<Joint> joints;

	std::map<std::string, Animation> animations;
	std::string currentAnimation;

	Skeleton();

	void AddJoint(Joint joint);
	int FindJointIndexByName(std::string name);
	void CreateAnimation(std::string animationName);
	Animation& GetCurrentAnimation(std::string currentAnimation_);
};
