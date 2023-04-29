#pragma once

#include "GridActor.h"

class Gear : public GridActor
{
public:
	ACTOR_SYSTEM(Gear);

	void Create() override;
	void OnLinkRotate() override;
};
