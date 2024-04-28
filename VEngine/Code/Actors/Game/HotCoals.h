#pragma once

#include "GridActor.h"

//Node grid actor that needs to be doused to step onto.
class HotCoals : public GridActor
{
public:
	ACTOR_SYSTEM(HotCoals);

	void Create() override;
	void Start() override;

	void Douse() override;
};
