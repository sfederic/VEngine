#include "vpch.h"
#include "SteamValve.h"
#include "Components/MeshComponent.h"
#include "Gameplay/GameUtils.h"

void SteamValve::Create()
{
	mesh->SetMeshFilename("valve.vmesh");
}

void SteamValve::OnLinkRotate()
{
	//@Todo: logic is thin here. Come back to it when you want to use it for something like a SteamSource actor
	GameUtils::SpawnParticleEmitter("Particle/smoke.png", GetPositionV());
	isValveOn = !isValveOn;
}
