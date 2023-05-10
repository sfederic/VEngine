#pragma once

#include "GridActor.h"

class Minecart : public GridActor
{
public:
	ACTOR_SYSTEM(Minecart);

	void Create() override;
	Properties GetProps() override;

	void OnLinkMove() override;
};
