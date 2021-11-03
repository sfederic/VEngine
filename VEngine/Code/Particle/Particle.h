#pragma once
#include "Transform.h"

struct Particle
{
	Transform transform;
	float lifetime = 0.f;
};
