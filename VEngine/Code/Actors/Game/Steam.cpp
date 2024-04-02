#include "vpch.h"
#include "Steam.h"
#include "Particle/ParticleEmitter.h"

Steam::Steam()
{
	steamEmitter = CreateComponent<ParticleEmitter>("SteamEmitter");
	rootComponent->AddChild(steamEmitter);
}

void Steam::Create()
{
	__super::Create();

	steamEmitter->SetTexture("Particle/smoke.png");
}

Properties Steam::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}

void Steam::Enable()
{
	SetActive(true);
}

void Steam::Disable()
{
	SetActive(false);
}
