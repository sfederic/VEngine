#include "vpch.h"
#include "GravityCrystal.h"
#include "Components/BoxTriggerComponent.h"
#include "Components/MeshComponent.h"
#include "Core/VMath.h"

GravityCrystal::GravityCrystal()
{
	gravityInfluenceTrigger = CreateComponent<BoxTriggerComponent>("BoxTrigger");
	rootComponent->AddChild(gravityInfluenceTrigger);
}

void GravityCrystal::Create()
{
	__super::Create();

	SetMeshFilename("crystal.vmesh");

	gravityInfluenceTrigger->SetExtents(2.f, 1.f, 2.f);
}

void GravityCrystal::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	auto containedActors = gravityInfluenceTrigger->GetAllContainedActors<GridActor>();
	for (auto actor : containedActors)
	{
		if (actor->GetCanFall())
		{
			previouslyContained.emplace(actor);

			actor->SetCanFall(false);

			const auto nextPos = actor->GetPositionV() + VMath::GlobalUpVector();
			actor->SetNextPos(nextPos);

			for (auto mesh : actor->GetComponents<MeshComponent>())
			{
				mesh->SetShaderItem(ShaderItems::Floating);
			}

			actor->RecalcCurrentNodeDontIgnoreThis();
		}
	}

	for (auto actorIt = previouslyContained.begin(); actorIt != previouslyContained.end();)
	{
		auto actor = (*actorIt);

		if (!gravityInfluenceTrigger->Contains(actor->GetPositionV()))
		{
			actor->SetCanFall(true);

			const auto nextPos = actor->GetPositionV() - VMath::GlobalUpVector();
			actor->SetNextPos(nextPos);

			for (auto mesh : actor->GetComponents<MeshComponent>())
			{
				mesh->SetShaderItem(ShaderItems::Default);
			}

			actor->RecalcCurrentNodeDontIgnoreThis();

			previouslyContained.erase(actor);

			//This is fucking stupid. The std::set iterator crashes if you erase in the for loop like this.
			if (previouslyContained.size() == 0)
			{
				return;
			}
		}
		else
		{
			++actorIt;
		}
	}
}
