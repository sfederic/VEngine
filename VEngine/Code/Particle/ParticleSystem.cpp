#include "vpch.h"
#include "ParticleSystem.h"
#include "ParticleEmitter.h"

ParticleSystem::ParticleSystem()
{
	auto emitter = ParticleEmitter::system.Add(this);
	AddEmitter(emitter);
	rootComponent = emitter;

	//@Todo: you can add more emitters here, might be nice to think about an "Add Component" editor button
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
