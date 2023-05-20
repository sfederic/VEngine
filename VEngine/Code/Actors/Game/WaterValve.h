#pragma once

#include "GridActor.h"

class WaterValve : public GridActor
{
public:
	ACTOR_SYSTEM(WaterValve);

	void Create() override;
	Properties GetProps() override;

	void OnLinkRotate() override;

private:
	std::string waterSourceName;
};
