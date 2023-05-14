#pragma once

#include "GridActor.h"

//Will rotate another Grid Actor this actor has reference to when Linked.
class Rotator : public GridActor
{
public:
	ACTOR_SYSTEM(Rotator);

	void Create() override;
	void Start() override;
	Properties GetProps() override;

	void OnLinkRotate() override;

private:
	std::string actorNameToRotate;
	GridActor* actorToRotate = nullptr;
};
