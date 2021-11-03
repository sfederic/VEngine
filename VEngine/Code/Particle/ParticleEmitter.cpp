#include "ParticleEmitter.h"

ParticleEmitter::ParticleEmitter()
{
	direction = XMFLOAT3(0.f, 1.f, 0.f);
}

void ParticleEmitter::CreateParticle(Particle particle)
{
	particles.emplace_back(particle);
}

void ParticleEmitter::Tick(float deltaTime)
{
	for (int i = 0; i < particles.size(); i++)
	{
		particles[i].transform.position.x += direction.x * (speed * deltaTime);
		particles[i].transform.position.y += direction.y * (speed * deltaTime);
		particles[i].transform.position.z += direction.z * (speed * deltaTime);
	}
}

void ParticleEmitter::Create()
{
}

Properties ParticleEmitter::GetProps()
{
	Properties props("ParticleEmitter");
	props.Add("Speed", &speed);
	props.Add("Texture Filename", &textureFilename);
	return props;
}
