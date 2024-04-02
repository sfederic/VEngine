#pragma once

#include "GridActor.h"

class ParticleEmitter;

//Hot steam, like from the kettle. Used as an obstacle to block player progression.
//The mesh in this actor isn't really used. Instead, the grid node recalcs are just using it as a reference.
class Steam : public GridActor
{
public:
	ACTOR_SYSTEM(Steam);

	Steam();
	void Start() override;
	void Create() override;
	Properties GetProps() override;

	void Enable();
	void Disable();
	void SetCurrentNodeState();

private:
	ParticleEmitter* steamEmitter = nullptr;
};
