#include "vpch.h"
#include "ParticleSystem.h"
#include "ParticleEmitter.h"
#include "Editor/ButtonProperty.h"

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

void ParticleSystem::NewEmitter()
{
	auto emitter = ParticleEmitter::system.Add(this);
	emitters.push_back(emitter);
}

Properties ParticleSystem::GetProps()
{
	auto props = __super::GetProps();
	props.Add("Add Emitter",
		new ButtonProperty(std::bind(&ParticleSystem::NewEmitter, this), "Add Emitter"));
	return props;
}
