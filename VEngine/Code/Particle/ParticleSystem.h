#pragma once

import Actors.Actor;
#include "Actors/ActorSystem.h"
#include "ParticleEmitter.h"
#include "Render/ShaderItem.h"

class ParticleSystem : public Actor
{
public:
	ACTOR_SYSTEM(ParticleSystem)

		ParticleSystem();
	void AddEmitter(ParticleEmitter* emitter);
	Properties GetProps() override;

private:
	std::vector<ParticleEmitter*> emitters;
};

template <typename ParticleType>
void SpawnParticleSystem(XMFLOAT3 position, float lifetime, std::string textureFilename, std::string shaderItemName = "DefaultClip")
{
	static_assert(std::is_convertible<ParticleType*, ParticleEmitter*>::value, "ParticleType must be derived from ParticleEmitter");

	auto emitter = (ParticleType*)ParticleEmitter::system.Add(nullptr, ParticleType(textureFilename, shaderItemName));
	emitter->SetPosition(position);
	emitter->emitterLifetime = lifetime;
}
