#include "vpch.h"
#include "ParticleSystem.h"
#include "ParticleEmitter.h"

ParticleSystem::ParticleSystem()
{
	auto emitter = ParticleEmitter::system.Add("Emitter", this);
	AddEmitter(emitter);
	SetRootComponent(emitter);
}

void ParticleSystem::AddEmitter(ParticleEmitter* emitter)
{
	emitters.emplace_back(emitter);
}

Properties ParticleSystem::GetProps()
{
	auto props = __super::GetProps();
	return props;
}
