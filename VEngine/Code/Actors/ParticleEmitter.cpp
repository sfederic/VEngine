#include "ParticleEmitter.h"
#include "Billboard.h"
#include "Components/EmptyComponent.h"

ParticleEmitter::ParticleEmitter()
{
	rootComponent = EmptyComponent::system.Add(this);
}

void ParticleEmitter::Tick(float deltaTime)
{
	timer += deltaTime;

	if (timer > 0.5)
	{
		Transform transform = rootComponent->transform;
		auto bill = Billboard::system.Add(Billboard(), transform);
		bills.push_back(bill);
		timer = 0.f;
	}

	for (auto bill : bills)
	{
		auto pos = bill->GetPosition();
		pos.y += deltaTime * 2.f;
		bill->SetPosition(pos);
	}
}

Properties ParticleEmitter::GetProps()
{
	return Properties();
}
