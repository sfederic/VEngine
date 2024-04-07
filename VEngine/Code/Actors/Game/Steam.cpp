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

	originalStartingSpawnRate = steamEmitter->particleData.spawnRate.y;
}

void Steam::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	//Lower the steam particle spawn rate 
	if (lowerParticleDensity)
	{
		steamEmitter->particleData.spawnRate.y += deltaTime * 5.5f;

		//Give the steam a nice little alpha fadeout
		const float alpha = steamEmitter->GetAlpha();
		if (alpha > 0.f)
		{
			const float emitterAlpha = alpha - (deltaTime * 2.5f);
			steamEmitter->SetAlpha(emitterAlpha);
		}

		constexpr float particleSpawnRateLimit = 10.f;
		if (steamEmitter->particleData.spawnRate.y > particleSpawnRateLimit)
		{
			steamEmitter->SetActive(false);
		}
	}
	else
	{
		const float alpha = steamEmitter->GetAlpha();
		if (alpha < 1.f)
		{
			const float emitterAlpha = alpha + (deltaTime * 4.5f);
			steamEmitter->SetAlpha(emitterAlpha);
		}
	}
}

void Steam::Create()
{
	__super::Create();

	steamEmitter->SetTexture("Particle/smoke.png");

	steamAudio->SetAudioFilename("steam.wav");
	steamAudio->SetLoop(true);

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
	steamEmitter->particleData.spawnRate.y = originalStartingSpawnRate;
	steamEmitter->SetActive(true);

	lowerParticleDensity = false;
	steamAudio->Play();
	SetCurrentNodeState();
}

void Steam::Disable()
{
	lowerParticleDensity = true;
	steamAudio->Stop();
	SetCurrentNodeState();
}

void Steam::SetCurrentNodeState()
{
	auto node = GetCurrentNode();

	if (!lowerParticleDensity) //Using this bool is not proper with the namaing, but serves the same purpose.
	{
		node->Hide();
	}
	else
	{
		node->Show();
	}
}
