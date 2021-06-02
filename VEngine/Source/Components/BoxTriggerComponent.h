#pragma once

#include "Component.h"

class Actor;

class BoxTriggerComponent : public Component
{
public:
	virtual void Tick(float deltaTime) override;
	virtual void Start() override;

private:
	Actor* box; //Links to DebugBox actor
};
