#include "vpch.h"
#include "Steam.h"
#include "Particle/ParticleEmitter.h"
#include "Components/MeshComponent.h"
#include "Components/AudioComponent.h"
#include "Gameplay/GridNode.h"

Steam::Steam()
{
	steamEmitter = CreateComponent<ParticleEmitter>("SteamEmitter");
	rootComponent->AddChild(steamEmitter);

	steamAudio = CreateComponent<AudioComponent>("SteamAudio");
	rootComponent->AddChild(steamAudio);
}

void Steam::Start()
{
	__super::Start();

	SetCurrentNodeState();
}

void Steam::Create()
{
	__super::Create();

	steamEmitter->SetTexture("Particle/smoke.png");

	steamAudio->SetAudioFilename("steam.wav");

	mesh->SetVisibility(false);
	mesh->SetActive(false);
	mesh->ignoreGridRaycasts = true;
	mesh->canBeLinkedTo = false;
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
	SetCurrentNodeState();
}

void Steam::Disable()
{
	SetActive(false);
	SetCurrentNodeState();
}

void Steam::SetCurrentNodeState()
{
	auto node = GetCurrentNode();

	//Don't get confused here. If the steam is On, hide the node.
	if (IsActive())
	{
		node->Hide();
	}
	else
	{
		node->Show();
	}
}
