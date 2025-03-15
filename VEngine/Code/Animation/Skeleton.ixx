export module Animation.Skeleton;

import Animation.Joint;
import Animation.Animation;
import <string>;
import <vector>;
import <cassert>;
import <map>;

export class Skeleton
{
public:
	void AddJoint(Joint& joint);
	Joint::JointIndex FindJointIndexByName(const std::string name);
	auto GetNumJoints() { return joints.size(); }
	void CreateAnimation(const std::string animationName);
	Animation& GetAnimation(const std::string animationName);

	auto& GetJoint(int jointIndex) { return joints.at(jointIndex); }
	auto& GetJoints() { return joints; }
	auto& GetAnimations() { return animations; }

private:
	std::vector<Joint> joints;
	std::map<std::string, Animation> animations;
};

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
