#include "ParticleEmitter.h"
#include "VMath.h"

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
	spawnTimer += deltaTime;
	if (spawnTimer > spawnRate)
	{
		Particle particle = {};
		//Need to get the world position instead of relative
		XMMATRIX worldMatrix = this->GetWorldMatrix();
		XMStoreFloat3(&particle.transform.position, worldMatrix.r[3]);
		particle.transform.position.x = VMath::RandomRange(-3.f, 3.f);
		CreateParticle(particle);

		spawnTimer = 0.f;
	}

	for (int i = 0; i < particles.size(); i++)
	{
		Particle& particle = particles[i];
		particle.lifetime += deltaTime;

		if (particle.lifetime > lifetime)
		{
			std::swap(particle, particles.back());
			particles.pop_back();
		}

		particle.transform.position.x += direction.x * (speed * deltaTime);
		particle.transform.position.y += direction.y * (speed * deltaTime);
		particle.transform.position.z += direction.z * (speed * deltaTime);
	}
}

void ParticleEmitter::Create()
{
}

Properties ParticleEmitter::GetProps()
{
	Properties props("ParticleEmitter");
	props.Add("Speed", &speed);
	props.Add("Spawn Rate", &spawnRate);
	props.Add("Lifetime", &lifetime);
	props.Add("Texture Filename", &textureFilename);
	return props;
}
