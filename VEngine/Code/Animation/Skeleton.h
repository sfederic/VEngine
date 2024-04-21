#pragma once

#include <vector>
#include <map>
#include "Joint.h"
#include "Animation.h"

class Skeleton
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

	//@Todo: need a "PLayAnimationOneShot" equivalent for animations on skeletal meshes. i.e. no looping.
	std::map<std::string, Animation> animations;
};
