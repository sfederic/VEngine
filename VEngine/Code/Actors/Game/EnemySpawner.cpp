#include "vpch.h"
#include "EnemySpawner.h"
#include "Components/BoxTriggerComponent.h"
#include "Components/MeshComponent.h"
#include "Actors/Game/Enemy.h"
#include "Core/Timer.h"

EnemySpawner::EnemySpawner()
{
	boxTrigger = CreateComponent(BoxTriggerComponent(), "BoxTrigger");
	boxTrigger->renderWireframeColour = XMFLOAT4(0.75f, 0.75f, 0.9f, 1.f);
	rootComponent = boxTrigger;
}

void EnemySpawner::Start()
{
	Timer::SetTimer(spawnInterval, std::bind(&EnemySpawner::SpawnEnemy, this), true);
}

Properties EnemySpawner::GetProps()
{
	auto props = __super::GetProps();
	props.title = "EnemySpawner";
	props.Add("Spawn Count", &numOfEnemiesToSpawn);
	props.Add("Spawn Interval", &spawnInterval);
	return props;
}

void EnemySpawner::SpawnEnemy()
{
	if (numOfEnemiesToSpawn <= 0) return;

	auto enemy = Enemy::system.Add();

	auto pointInBoundingBox = boxTrigger->GetRandomPointInTriggerRounded();

	//Keep vector of previous points to re-check them on Contains() == true below
	std::vector<XMVECTOR> previousContainedPoints;

	//Retry random point in bounds if it's inside another bounds
	for (auto& mesh : MeshComponent::system.GetComponents())
	{
		if (mesh->boundingBox.Contains(pointInBoundingBox))
		{
			previousContainedPoints.emplace_back(pointInBoundingBox);

			pointInBoundingBox = boxTrigger->GetRandomPointInTriggerRounded();
			for (auto& previousPoint : previousContainedPoints)
			{
				if (XMVector4Equal(pointInBoundingBox, previousPoint))
				{
					return; //@Todo: this is a lazy way to avoid enemies being spawned inside other things
				}
			}
		}
	}

	enemy->SetPosition(pointInBoundingBox);

	numOfEnemiesToSpawn--;
}
