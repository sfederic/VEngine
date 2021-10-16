#pragma once
#include "SpatialComponent.h"
#include "ComponentSystem.h"
#include <string>

struct AudioComponent : SpatialComponent
{
	COMPONENT_SYSTEM(AudioComponent)

	std::string audioFilename;
	uint64_t channelID = 0;
	bool playOnStart = false;

	AudioComponent();
	virtual void Tick(double deltaTime) override;
	virtual void Start() override;
	virtual void Create() override;
	virtual Properties GetProps() override;
};
