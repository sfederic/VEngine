#include "ParticleSystem.h"
#include "Components/EmptyComponent.h"
#include "ParticleEmitter.h"

ParticleSystem::ParticleSystem()
{
	rootComponent = EmptyComponent::system.Add(this);

	auto emitter = ParticleEmitter::system.Add(this);
	emitter->textureFilename = "bush.png";
	emitter->CreateParticle(Particle());
	AddEmitter(emitter);
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
