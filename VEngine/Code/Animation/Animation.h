#pragma once

#include <map>
#include <vector>
#include "AnimFrame.h"
#include "Joint.h"

class Animation
{
public:
	static inline const int ANIM_NAME_MAX = 128;

	Animation(const char* name_)
	{
		SetName(name_);
	}

	void SetName(const char* name_)
	{
		strcpy_s(name, sizeof(char) * ANIM_NAME_MAX, name_);
	}

	std::string GetName() const { return name; }

	double GetStartTime(int jointIndex)
	{
		const auto& frame = frames.find(jointIndex)->second;
		if (frame.empty())
		{
			return 0.f;
		}
		return frame.front().time;
	}

	double GetEndTime(int jointIndex)
	{
		const auto& frame = frames.find(jointIndex)->second;
		if (frame.empty())
		{
			return 0.f;
		}
		return frame.back().time;
	}

	auto& GetFrame(int jointIndex) { return frames.find(jointIndex)->second; }
	auto& GetFrames() { return frames; }

	void AddFrame(Joint::JointIndex index, std::vector<AnimFrame>& frame) { frames.emplace(index, frame); }
	bool HasFrames() { return frames.size() > 0; }

	float GetFinalTime();

	void Interpolate(float t, Joint& joint, std::vector<Joint>& joints);

private:
	char name[ANIM_NAME_MAX]{};
	std::map<Joint::JointIndex, std::vector<AnimFrame>> frames;
	bool isPlaying = true;
};
