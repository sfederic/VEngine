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

		//Set position from radius
		particle.transform.position.x += VMath::RandomRange(-spawnRadius, spawnRadius);
		particle.transform.position.y += VMath::RandomRange(-spawnRadius, spawnRadius);
		particle.transform.position.z += VMath::RandomRange(-spawnRadius, spawnRadius);

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

		particle.transform.position.x += direction.x * (moveSpeed * deltaTime);
		particle.transform.position.y += direction.y * (moveSpeed * deltaTime);
		particle.transform.position.z += direction.z * (moveSpeed * deltaTime);
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
	props.Add("Texture", &textureFilename);
	props.Add("Direction", &direction);
	props.Add("Rotate", &rotation);
	props.Add("Rotate Speed", &rotateSpeed);
	return props;
}
