#pragma once

#include "GridActor.h"

//Different from a MultiOrientationLock in that the correct orientations will activate two actors
//at different set axis rather than waiting for all actors to be aligned to activate an actor.
class TwinOrientationLock : public GridActor
{
public:
	ACTOR_SYSTEM(TwinOrientationLock);

	void Start() override;
	void Tick(float deltaTime) override;
	Properties GetProps() override;

private:
	void ActivateActorsOnCorrectOrientations();

	std::string actorName0;
	std::string actorName1;

	//The orientation and actor numbers correlate with each other.
	Actor* actor0 = nullptr;
	Actor* actor1 = nullptr;

	//Orientations will have to work off of ony one axis on this actor (e.g. up, right forward).
	XMFLOAT3 orientation0 = XMFLOAT3(1.f, 0.f, 0.f);
	XMFLOAT3 orientation1 = XMFLOAT3(-1.f, 0.f, 0.f);
};
