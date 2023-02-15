#pragma once

#include <string>
#include "AnimationValues.h"

//Sockets currently on support being attached to a single joint.
class JointSocket
{
public:
	JointSocket(int jointIndex_, std::string_view name_) :
		jointIndex(jointIndex_), name(name_) {}

private:
	std::string name;
	int jointIndex = INVALID_JOINT_INDEX;
};
