#pragma once

#include "GridActor.h"

//Grid actor that fuses onto another grid actor. Think like Tears of the Kingdom's fuse ability.
//FuseGridActors will probably have to be cubic to work best.
class FuseGridActor : public GridActor
{
public:
	ACTOR_SYSTEM(FuseGridActor);

	Properties GetProps() override;
	void OnLinkMove() override;
};
