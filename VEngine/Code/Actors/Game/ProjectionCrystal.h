#pragma once

#include "GridActor.h"

//Projects a 'scene' into the world.
class ProjectionCrystal : public GridActor
{
public:
	ACTOR_SYSTEM(ProjectionCrystal);

	void Interact() override;
	void OnLinkActivate() override;
	void OnLinkDeactivate() override;

private:
	bool isActive = false;
};
