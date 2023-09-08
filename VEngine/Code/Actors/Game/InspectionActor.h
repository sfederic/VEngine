#pragma once

#include "GridActor.h"

//Actor that zooms the camera in on player inspection.
class InspectionActor : public GridActor
{
public:
	ACTOR_SYSTEM(InspectionActor);

	void Create() override;
	void Inspect() override;
};
