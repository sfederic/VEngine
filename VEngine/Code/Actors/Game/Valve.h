#pragma once

#include "GridActor.h"

class Valve : public GridActor
{
public:
	ACTOR_SYSTEM(Valve);

	void Create() override;
	Properties GetProps() override;

	void OnLinkRotate() override;

private:
	std::string waterSourceName;
};
