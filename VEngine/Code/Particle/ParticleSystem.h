#pragma once

#include "Actors/Actor.h"
#include "Actors/ActorSystem.h"
#include "Timer.h"
#include "ParticleEmitter.h"
#include "Render/ShaderItem.h"

struct ParticleSystem : Actor
{
	ACTOR_SYSTEM(ParticleSystem)

	std::vector<ParticleEmitter*> emitters;
	
	ParticleSystem();
	void AddEmitter(ParticleEmitter* emitter);
	virtual Properties GetProps() override;
};

template <typename ParticleType>
void SpawnParticleSystem(XMFLOAT3 position, float lifetime, std::string textureFilename, ShaderItem* shaderItem = ShaderItems::DefaultClip)
{
	static_assert(std::is_convertible<ParticleType*, ParticleEmitter*>::value, "ParticleType must be derived from ParticleEmitter");

	auto emitter = (ParticleType*)ParticleEmitter::system.Add(nullptr, ParticleType(textureFilename, shaderItem));
	emitter->SetPosition(position);
	emitter->emitterLifetime = lifetime;
}
