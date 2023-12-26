#pragma once

#include "GridActor.h"

class ParticleEmitter;

class SteamValve : public GridActor
{
public:
	ACTOR_SYSTEM(SteamValve);

	void Create() override;
	void Start() override;
	Properties GetProps() override;

	void OnLinkRotate() override;

private:
	ParticleEmitter* steamEmitter = nullptr;
	bool isValveOn = false;
};
