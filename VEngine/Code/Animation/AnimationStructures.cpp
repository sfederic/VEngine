#include "AnimationStructures.h"

//@Todo: the args here feel shitty. Find a way to put in all in the joint.
void Animation::Interpolate(float t, Joint& joint, Skeleton* skeleton)
{
	for (int i = 0; i < (frames.size() - 1); i++)
	{
		if (t >= frames[i].time && t <= frames[i + 1].time)
		{
			//Get the interpolation difference between input time and frame time.
			float lerpPercent = (t - frames[i].time) / (frames[i + 1].time - frames[i].time);

			XMVECTOR currentPos = XMLoadFloat3(&frames[i].pos);
			XMVECTOR nextPos = XMLoadFloat3(&frames[i + 1].pos);

			XMVECTOR currentScale = XMLoadFloat3(&frames[i].scale);
			XMVECTOR nextScale = XMLoadFloat3(&frames[i + 1].scale);

			XMVECTOR currentRot = XMLoadFloat4(&frames[i].rot);
			XMVECTOR nextRot = XMLoadFloat4(&frames[i + 1].rot);

			XMVECTOR lerpedPos = XMVectorLerp(currentPos, nextPos, lerpPercent);
			XMVECTOR lerpedScale = XMVectorLerp(currentScale, nextScale, lerpPercent);
			XMVECTOR lerpedRot = XMQuaternionSlerp(currentRot, nextRot, lerpPercent);
			XMVECTOR zero = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

			joint.currentPose = XMMatrixAffineTransformation(lerpedScale, zero, lerpedRot, lerpedPos);

			int parentIndex = joint.parentIndex;
			while (parentIndex > 0)
			{
				Joint& parentJoint = skeleton->joints[parentIndex];
				joint.currentPose *= parentJoint.currentPose;
				parentIndex = parentJoint.parentIndex;
			}

			return;
		}
	}
}

void Skeleton::AddJoint(Joint joint)
{
	joints.push_back(joint);
	joints.back().index = joints.size() - 1;
}

int Skeleton::FindJointIndexByName(std::string name)
{
	for (int i = 0; i < joints.size(); i++)
	{
		if (joints[i].name == name)
		{
			return joints[i].index;
		}
	}

	return -1;
}
