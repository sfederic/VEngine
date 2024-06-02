#pragma once

#include "GridActor.h"

class Lever : public GridActor
{
public:
	ACTOR_SYSTEM(Lever);

	void Create() override;
	void Start() override;
	Properties GetProps() override;

	void OnLinkRotateUp() override;
	void OnLinkRotateDown() override;

private:
	std::string actorToActivateName;
	Actor* actorToActivate = nullptr;

	bool isLeverActive = false;
};
