#include "ParticleEmitter.h"
#include "VMath.h"

ParticleEmitter::ParticleEmitter()
{
}

void ParticleEmitter::CreateParticle(Particle particle)
{
	particles.emplace_back(particle);
}

void ParticleEmitter::Tick(float deltaTime)
{
	spawnTimer += deltaTime;
	float spawnRateRange = VMath::RandomRange(spawnRate.x, spawnRate.y);
	if (spawnTimer > spawnRateRange)
	{
		Particle particle = {};

		//Need to get the world position instead of relative
		XMMATRIX worldMatrix = this->GetWorldMatrix();
		XMStoreFloat3(&particle.transform.position, worldMatrix.r[3]);

		//Set position from radius
		particle.transform.position.x += VMath::RandomRange(spawnRadius.x, spawnRadius.y);
		particle.transform.position.y += VMath::RandomRange(spawnRadius.x, spawnRadius.y);
		particle.transform.position.z += VMath::RandomRange(spawnRadius.x, spawnRadius.y);

		//Get random range between move speeds
		float moveSpeedRange = VMath::RandomRange(moveSpeed.x, moveSpeed.y);
		particle.moveSpeed = moveSpeedRange;

		//Set particle rotation values
		float rotateRange = VMath::RandomRange(rotation.x, rotation.y);
		particle.angle = rotateRange;

		float rotateSpeedRange = VMath::RandomRange(rotateSpeed.x, rotateSpeed.y);
		particle.rotateSpeed = rotateSpeedRange;

		CreateParticle(particle);

		spawnTimer = 0.f;
	}

	for (int i = 0; i < particles.size(); i++)
	{
		Particle& particle = particles[i];
		particle.lifetime += deltaTime;

		//Get random range between lifetimes
		float lifetimeRange = VMath::RandomRange(lifetime.x, lifetime.y);

		if (particle.lifetime > lifetimeRange)
		{
			std::swap(particle, particles.back());
			particles.pop_back();
		}

		particle.AddVelocity(direction, deltaTime);
	}

	emitterLifetimeTimer += deltaTime;
	if (emitterLifetime > 0.f && emitterLifetimeTimer > emitterLifetime)
	{
		this->Remove();
	}
}

void ParticleEmitter::Create()
{
}

Properties ParticleEmitter::GetProps()
{
	Properties props("ParticleEmitter");
	props.Add("Move Speed", &moveSpeed);
	props.Add("Spawn Rate", &spawnRate);
	props.Add("Spawn Radius", &spawnRadius);
	props.Add("Lifetime", &lifetime);
	props.Add("Texture", &textureData);
	props.Add("Direction", &direction);
	props.Add("Rotate", &rotation);
	props.Add("Rotate Speed", &rotateSpeed);
	return props;
}
