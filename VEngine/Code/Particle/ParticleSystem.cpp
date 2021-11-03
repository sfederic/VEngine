#include "ParticleSystem.h"
#include "ParticleEmitter.h"

ParticleSystem::ParticleSystem()
{
	auto emitter = ParticleEmitter::system.Add(this);
	//emitter->textureFilename = "bush.png";
	emitter->direction.y = -1.f;
	AddEmitter(emitter);

	rootComponent = emitter;
}

void ParticleSystem::AddEmitter(ParticleEmitter* emitter)
{
	emitters.push_back(emitter);
}

Properties ParticleSystem::GetProps()
{
	auto props = Actor::GetProps();
	return props;
}
