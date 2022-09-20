#include "vpch.h"
#include "Enemy.h"
#include "Components/BoxTriggerComponent.h"
#include "Components/MeshComponent.h"
#include "Gameplay/GameUtils.h"

Enemy::Enemy()
{
	aggroTrigger = CreateComponent(BoxTriggerComponent(), "AggroTrigger");

	mesh = CreateComponent(MeshComponent(), "Mesh");
	rootComponent = mesh;
	rootComponent->AddChild(aggroTrigger);
}

void Enemy::Start()
{
	aggroTrigger->SetTargetAsPlayer();
}

void Enemy::Tick(float deltaTime)
{
	if (aggroTrigger->ContainsTarget() && !inCombat)
	{
		PlayerEnteredAggroTrigger();
	}
}

Properties Enemy::GetProps()
{
	return __super::GetProps();
}

void Enemy::PlayerEnteredAggroTrigger()
{
	inCombat = true;
	Log("Combat started with Enemy [%s].", GetName().c_str());
	GameUtils::SetPlayerCombatOn();
	aggroTrigger->renderWireframeColour = XMFLOAT4(1.f, 0.f, 0.f, 1.f); //Set trigger to red.
}
