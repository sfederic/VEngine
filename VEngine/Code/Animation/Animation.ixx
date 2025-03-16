#include <DirectXMath.h>

export module Animation.Animation;

import Animation.Joint;
import Animation.AnimFrame;
import <string>;
import <map>;
import <vector>;

using namespace DirectX;

export class Animation
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

float Animation::GetFinalTime()
{
	float highestTime = 0.f;

	for (auto& framePair : frames)
	{
		for (auto& frame : framePair.second)
		{
			if (frame.time > highestTime)
			{
				highestTime = frame.time;
			}
		}
	}

	return highestTime;
}

void Animation::Interpolate(float t, Joint& joint, std::vector<Joint>& joints)
{
	if (!isPlaying)
	{
		return;
	}

	//get anim frames connected to joint
	std::vector<AnimFrame>& jointFrames = frames[joint.index];

	if (jointFrames.empty())
	{
		return;
	}

	for (size_t i = 0; i < (jointFrames.size() - 1); i++)
	{
		if (t >= jointFrames[i].time && t <= jointFrames[i + 1].time)
		{
			//Get the interpolation difference between input time and frame time.
			float lerpPercent = (t - jointFrames[i].time) / (jointFrames[i + 1].time - jointFrames[i].time);

			XMVECTOR currentPos = XMLoadFloat3(&jointFrames[i].pos);
			XMVECTOR currentScale = XMLoadFloat3(&jointFrames[i].scale);
			XMVECTOR currentRot = XMLoadFloat4(&jointFrames[i].rot);

			XMVECTOR nextPos = XMLoadFloat3(&jointFrames[i + 1].pos);
			XMVECTOR nextScale = XMLoadFloat3(&jointFrames[i + 1].scale);
			XMVECTOR nextRot = XMLoadFloat4(&jointFrames[i + 1].rot);

			XMVECTOR lerpedPos = XMVectorLerp(currentPos, nextPos, lerpPercent);
			XMVECTOR lerpedScale = XMVectorLerp(currentScale, nextScale, lerpPercent);
			XMVECTOR lerpedRot = XMQuaternionSlerp(currentRot, nextRot, lerpPercent);
			XMVECTOR zero = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

			joint.currentPose = XMMatrixAffineTransformation(lerpedScale, zero, lerpedRot, lerpedPos);

			XMMATRIX endPose = joint.currentPose;
			int parentIndex = joint.parentIndex;

			while (parentIndex > -1)
			{
				Joint& parentJoint = joints[parentIndex];
				endPose *= parentJoint.currentPose;

				parentIndex = parentJoint.parentIndex;
			}

			joint.currentPose = joint.inverseBindPose * endPose;

			return;
		}
	}
}
