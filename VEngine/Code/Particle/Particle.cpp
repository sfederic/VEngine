
#include "Particle.h"
import Core.VMath;
#include "ParticleData.h"

void Particle::SetParticleRangeData(ParticleData& particleData)
{
	//Set position from radius
	transform.position.x += VMath::RandomRange(particleData.spawnRadius.x, particleData.spawnRadius.y);
	transform.position.y += VMath::RandomRange(particleData.spawnRadius.x, particleData.spawnRadius.y);
	transform.position.z += VMath::RandomRange(particleData.spawnRadius.x, particleData.spawnRadius.y);

	//Get random range between move speeds
	const float moveSpeedRange = VMath::RandomRange(particleData.moveSpeed.x, particleData.moveSpeed.y);
	moveSpeed = moveSpeedRange;

	//Set particle rotation values
	const float rotateRange = VMath::RandomRange(particleData.rotation.x, particleData.rotation.y);
	angle = rotateRange;

	const float rotateSpeedRange = VMath::RandomRange(particleData.rotateSpeed.x, particleData.rotateSpeed.y);
	rotateSpeed = rotateSpeedRange;

	const XMFLOAT3 directionRange = VMath::RandomRangeFloat3(particleData.minDirection, particleData.maxDirection);
	direction = directionRange;
}
