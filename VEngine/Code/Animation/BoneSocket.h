#pragma once

#include "AnimationValues.h"

class BoneSocket
{
public:
	BoneSocket(int boneIndex_) : boneIndex(boneIndex_) {}

private:
	int boneIndex = INVALID_BONE_INDEX;
};
