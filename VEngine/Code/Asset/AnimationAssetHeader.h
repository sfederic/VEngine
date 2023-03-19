#pragma once

#include "Animation/Animation.h"

//Binary header structure for skeletal animation asset files
struct AnimationAssetHeader
{
	char name[Animation::ANIM_NAME_MAX]{};
	uint64_t frameCount = 0;
};
