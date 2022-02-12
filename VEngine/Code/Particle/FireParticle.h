#pragma once
#include "ParticleEmitter.h"

struct FireParticle : ParticleEmitter
{
	COMPONENT_SYSTEM(FireParticle);

	FireParticle()
	{
		textureData.filename = "fire2.png";
	}
};
