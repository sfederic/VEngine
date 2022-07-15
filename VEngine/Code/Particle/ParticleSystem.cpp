#include "vpch.h"
#include "ParticleSystem.h"
#include "ParticleEmitter.h"

ParticleSystem::ParticleSystem()
{
	auto emitter = ParticleEmitter::system.Add(this);
	AddEmitter(emitter);
	rootComponent = emitter;
}

void ParticleSystem::AddEmitter(ParticleEmitter* emitter)
{
	emitters.push_back(emitter);
}

Properties ParticleSystem::GetProps()
{
	auto props = __super::GetProps();
	return props;
}
