#pragma once

#include "GridActor.h"

//@Todo: this actor needs some limitations on how it'll be rotated. A lever can't be rotated 360 degrees.
class Lever : public GridActor
{
public:
	ACTOR_SYSTEM(Lever);

	void Create() override;
	void Start() override;
	Properties GetProps() override;

	void OnLinkRotate() override;

private:
	std::string actorToActivateName;
	Actor* actorToActivate = nullptr;

	bool isLeverActive = false;
};
