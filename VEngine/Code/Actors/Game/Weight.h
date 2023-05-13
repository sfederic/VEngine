#pragma once

#include "GridActor.h"

class Weight : public GridActor
{
public:
	ACTOR_SYSTEM(Weight);

	void Tick(float deltaTime) override;

private:
	void CrushBreakableGridActorUnderneath();
};
