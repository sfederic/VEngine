#pragma once

#include "GridActor.h"

//Resets player bullet count to max.
class BulletCache : public GridActor
{
public:
	ACTOR_SYSTEM(BulletCache);

	BulletCache();
	virtual Properties GetProps() override;
	virtual void Interact() override;
};
