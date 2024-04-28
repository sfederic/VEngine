#pragma once

#include "Actor.h"
#include "ActorSystem.h"

class MeshComponent;

class SkyBox : public Actor
{
public:
	ACTOR_SYSTEM(SkyBox);

	SkyBox();
	void Create() override;
	Properties GetProps() override;

private:
	MeshComponent* skyboxMesh = nullptr;
};
