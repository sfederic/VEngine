#include "vpch.h"
#include "HitResult.h"
#include "Components/MeshComponent.h"
#include "Actors/Game/Player.h"

Actor* HitResult::FindHitActor(Actor* findActor)
{
	for (auto actor : hitActors)
	{
		if (actor == findActor)
		{
			return actor;
		}
	}

	return nullptr;
}

void HitResult::AddActorsToIgnore(std::vector<Actor*>& actors)
{
	actorsToIgnore.reserve(actorsToIgnore.size() + actors.size());

	for (auto actor : actors)
	{
		actorsToIgnore.emplace_back(actor);
	}
}

void HitResult::IgnorePlayer()
{
	actorsToIgnore.emplace_back(Player::system.GetOnlyActor());
}

void HitResult::AddAllRenderStaticMeshesToIgnore()
{
	for (auto& mesh : MeshComponent::system.GetComponents())
	{
		if (!mesh->IsRenderStatic())
		{
			componentsToIgnore.emplace_back(mesh.get());
		}
	}
}
