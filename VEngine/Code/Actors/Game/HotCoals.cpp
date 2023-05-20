#include "vpch.h"
#include "HotCoals.h"
#include "Components/MeshComponent.h"
#include "Gameplay/GameUtils.h"

void HotCoals::Create()
{
	mesh->SetMeshFilename("node.vmesh");
	mesh->SetTexture("hot_coals.jpg");
}

void HotCoals::Douse()
{
	GameUtils::SpawnParticleEmitter("Particle/smoke.png", GetPositionV(), 1.5f);
	Destroy();
}
