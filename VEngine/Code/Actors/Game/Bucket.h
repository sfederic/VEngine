#pragma once

#include "GridActor.h"

class Bucket : public GridActor
{
public:
	ACTOR_SYSTEM(Bucket);

	void Create() override;
	void Tick(float deltaTime) override;
	Properties GetProps() override;

private:
	bool isFilled = false;
};
