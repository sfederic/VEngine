#pragma once

#include "AnimationValues.h"

//Sockets currently on support being attached to a single joint.
class JointSocket
{
public:
	JointSocket(int jointIndex_) : jointIndex(jointIndex_) {}

private:
	int jointIndex = INVALID_JOINT_INDEX;
};
