#include "vpch.h"
#include "Skeleton.h"

void Skeleton::AddJoint(Joint& joint)
{
	for (auto& existingjoint : joints)
	{
		assert(strcmp(existingjoint.name, joint.name) != 0);
	}

	joints.emplace_back(joint);
	joints.back().index = joints.size() - 1;
}

Joint::JointIndex Skeleton::FindJointIndexByName(const std::string name)
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
