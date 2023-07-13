#pragma once

#include "GridActor.h"

class Lift;

//Used to move a Lift grid actor.
class LiftCrank : public GridActor
{
public:
	ACTOR_SYSTEM(LiftCrank);

	void Create() override;
	void Start() override;

	void OnLinkRotateLeft() override;
	void OnLinkRotateRight() override;

private:
	void MoveLinkedLiftAndContainedActors(XMVECTOR moveDirection);

	std::string linkedLiftName;
	Lift* linkedLift = nullptr;
};
