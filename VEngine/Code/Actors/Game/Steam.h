#pragma once

#include "GridActor.h"

class ParticleEmitter;
class AudioComponent;

//Hot steam, like from the kettle. Used as an obstacle to block player progression.
//The mesh in this actor isn't really used. Instead, the grid node recalcs are just using it as a reference.
class Steam : public GridActor
{
public:
	ACTOR_SYSTEM(Steam);

	Steam();
	void Create() override;
	void Start() override;
	void Tick(float deltaTime) override;
	Properties GetProps() override;

	void Enable();
	void Disable();
	void SetCurrentNodeState();

private:
	ParticleEmitter* steamEmitter = nullptr;
	AudioComponent* steamAudio = nullptr;

	float originalStartingSpawnRate = 0.f;

	//Use this when turning off steam so it gradually turns off by lowering the emitter spawn rate.
	bool steamEnabled = false;
};
