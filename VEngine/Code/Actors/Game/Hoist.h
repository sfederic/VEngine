#pragma once

#include "GridActor.h"

//A hoist that holds onto another Actor, causing it to fall down when this Hoist is attacked.
//(Think of it like a pulley hoist)
class Hoist : public GridActor
{
public:
	ACTOR_SYSTEM(Hoist);

	Hoist();
	virtual void Attacked() override;
	virtual Properties GetProps() override;

private:
	std::string hoistedActorName;
};
