#pragma once

#include <map>
#include <vector>
#include "AnimFrame.h"
#include "Joint.h"

class Skeleton;

class Animation
{
public:
	Animation(const char* name_)
	{
		SetName(name_);
	}

	void SetName(const char* name_)
	{
		strcpy_s(name, sizeof(char) * ANIM_NAME_MAX, name_);
	}

	std::string GetName() { return name; }

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

	auto& GetFrame(int jointIndex) { return frames.find(jointIndex)->second; }
	auto& GetFrames() { return frames; }

	void AddFrame(Joint::JointIndex index, std::vector<AnimFrame>& frame) { frames.emplace(index, frame); }
	bool HasFrames() { return frames.size() > 0; }

	float GetFinalTime();

	void Interpolate(float t, Joint& joint, Skeleton* skeleton);

	static inline const int ANIM_NAME_MAX = 128;

private:
	char name[ANIM_NAME_MAX]{};
	std::map<Joint::JointIndex, std::vector<AnimFrame>> frames;
	bool isPlaying = true;
};
