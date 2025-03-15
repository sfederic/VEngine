export module Animation.Skeleton;

import <string>;
import <vector>;
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
