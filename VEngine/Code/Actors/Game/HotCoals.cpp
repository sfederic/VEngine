
#include "HotCoals.h"
import Components.MeshComponent;
#include "Gameplay/GameUtils.h"
#include "Gameplay/GridNode.h"

void HotCoals::Create()
{
	__super::Create();

	_mesh->SetMeshFilename("node.vmesh");
	_mesh->SetTexture("hot_coals.jpg");
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
