#pragma once

#include "Actor.h"
#include "ActorSystem.h"

struct BoxTriggerComponent;
class MeshComponent;

//Meshes within this volume will only be lit by lights also within this volume.
//Use this to block out unwanted lights in closed off areas.
class LightVolume : public Actor
{
public:
	ACTOR_SYSTEM(LightVolume);

	LightVolume();
	Properties GetProps() override;

	//Iterate through every light in the scene and disable if not inside this volume.
	void EnableLightsIfInsideVolume();
	void EnableAllLights();
	bool IsMeshWithinVolume(MeshComponent* mesh);

private:
	BoxTriggerComponent* boxTrigger = nullptr;
};
