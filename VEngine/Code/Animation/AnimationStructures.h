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
	XMFLOAT4 rot = XMFLOAT4(0.f, 0.f, 0.f, 1.f); //Quaternion
	XMFLOAT3 pos = XMFLOAT3(0.f, 0.f, 0.f);
	XMFLOAT3 scale = XMFLOAT3(1.f, 1.f, 1.f);
	double time = 0.0;
};

struct Animation
{
	bool isPlaying = true;

	static inline const int ANIM_NAME_MAX = 128;

	char name[ANIM_NAME_MAX]{};

	//Maps joint index to AnimFrames
	std::map<int, std::vector<AnimFrame>> frames;

	Animation(const char* name_)
	{
		SetName(name_);
	}

	void SetName(const char* name_)
	{
		strcpy_s(name, sizeof(char) * ANIM_NAME_MAX, name_);
	}

	float GetStartTime(int jointIndex)
	{
		if (frames[jointIndex].empty()) return 0.f;
		return frames[jointIndex].front().time;
	}

	float GetEndTime(int jointIndex)
	{
		if (frames[jointIndex].empty()) return 0.f;
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

	char name[32]{};

	int parentIndex = -1; //-1 is the root joint's index or if the bone doesn't have a parent
	int index = 0;
};

struct Skeleton
{
	std::vector<Joint> joints;

	std::map<std::string, Animation> animations;

	void AddJoint(Joint joint);
	int FindJointIndexByName(std::string name);
	size_t GetNumJoints() { return joints.size(); }
	void CreateAnimation(const std::string animationName);
	Animation& GetAnimation(const std::string animationName);
};
