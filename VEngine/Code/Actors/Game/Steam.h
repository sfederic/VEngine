#pragma once

#include "GridActor.h"

class ParticleEmitter;

//Hot steam, like from the kettle. Used as an obstacle to block player progression.
class Steam : public GridActor
{
public:
	ACTOR_SYSTEM(Steam);

	Steam();
	void Create() override;
	Properties GetProps() override;

	void Enable();
	void Disable();

private:
	ParticleEmitter* steamEmitter = nullptr;
};
