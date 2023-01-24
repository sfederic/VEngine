#pragma once

#include "Components/SpatialComponent.h"
#include "Components/ComponentSystem.h"

class MeshParticleEmitter : public SpatialComponent
{
public: 
	COMPONENT_SYSTEM(MeshParticleEmitter);
};
