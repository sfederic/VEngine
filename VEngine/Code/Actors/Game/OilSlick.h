#pragma once

#include "GridActor.h"

class OilSlick : public GridActor
{
public:
	ACTOR_SYSTEM(OilSlick);

	void Create() override;
	Properties GetProps() override;
};
