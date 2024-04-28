#include "vpch.h"
#include "HotCoals.h"
#include "Components/MeshComponent.h"
#include "Gameplay/GameUtils.h"
#include "Gameplay/GridNode.h"

void HotCoals::Create()
{
	__super::Create();

	mesh->SetMeshFilename("node.vmesh");
	mesh->SetTexture("hot_coals.jpg");
}

void HotCoals::Start()
{
	__super::Start();

	GetCurrentNode()->Hide();
}

void HotCoals::Douse()
{
	__super::Douse();
	GameUtils::SpawnParticleEmitter("Particle/smoke.png", GetPositionV(), 1.5f);
	GetCurrentNode()->Show();
	RecalcCurrentNodePosition();
	Remove();
}
