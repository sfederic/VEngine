#include "vpch.h"
#include "AnimationStructures.h"

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

void Animation::Interpolate(float t, Joint& joint, Skeleton* skeleton)
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
				Joint& parentJoint = skeleton->joints[parentIndex];
				endPose *= parentJoint.currentPose;

				parentIndex = parentJoint.parentIndex;
			}

			joint.currentPose = joint.inverseBindPose * endPose;

			return;
		}
	}
}

void Skeleton::AddJoint(Joint joint)
{
	joints.emplace_back(joint);
	//@Todo: assert on joint not having the same name as existing joint. Encapsulation will need to be fixed.
	joints.back().index = joints.size() - 1;
}

JointIndex Skeleton::FindJointIndexByName(const std::string name)
{
	for (int i = 0; i < joints.size(); i++)
	{
		if (joints[i].name == name)
		{
			return joints[i].index;
		}
	}

	throw new std::exception("Joint index not found");
}

void Skeleton::CreateAnimation(const std::string animationName)
{
	assert(animations.find(animationName) == animations.end());
	animations.emplace(animationName, Animation(animationName.c_str()));
}

Animation& Skeleton::GetAnimation(const std::string animationName)
{
	return animations.find(animationName)->second;
}
