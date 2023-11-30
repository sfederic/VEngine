#pragma once

#include "Animation/Animation.h"

struct AnimationFrameHeader
{
	char name[Animation::ANIM_NAME_MAX]{};
	uint64_t frameCount = 0;
};

//Binary header structure for skeletal animation asset files
struct AnimationAssetHeader
{
	uint64_t animationCount = 0;
};
