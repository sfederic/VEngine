#include "vpch.h"
#include "SteamValve.h"
#include "Gameplay/GameUtils.h"
#include "Particle/ParticleEmitter.h"

void SteamValve::Create()
{
	__super::Create();

	ignoreRotationValidCheck = true;
	canBeMovedInLink = false;

	rotateSpeed = 2.f;

	SetMeshFilename("gear.vmesh");
}

void SteamValve::Start()
{
	__super::Start();

	steamEmitter = GameUtils::SpawnParticleEmitter("Particle/smoke.png", GetPositionV());
	steamEmitter->SetActive(isValveOn);
}

Properties SteamValve::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("Valve On", &isValveOn);
	return props;
}

void SteamValve::OnLinkRotate()
{
	__super::OnLinkRotate();

	isValveOn = !isValveOn;
	steamEmitter->SetActive(isValveOn);
}
