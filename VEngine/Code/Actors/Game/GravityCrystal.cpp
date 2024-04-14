#include "vpch.h"
#include "GravityCrystal.h"
#include "Components/BoxTriggerComponent.h"
#include "Components/MeshComponent.h"
#include "Core/VMath.h"
#include "Render/BlendStates.h"

GravityCrystal::GravityCrystal()
{
	gravityFieldMesh = CreateComponent<MeshComponent>("GravityFieldMesh");
	rootComponent->AddChild(gravityFieldMesh);

	gravityInfluenceTrigger = CreateComponent<BoxTriggerComponent>("BoxTrigger");
	rootComponent->AddChild(gravityInfluenceTrigger);
}

void GravityCrystal::Create()
{
	__super::Create();

	SetMeshFilename("crystal.vmesh");

	gravityInfluenceTrigger->SetExtents(1.f, 1.f, 1.f);

	gravityFieldMesh->SetMeshFilename("cube.vmesh");
	gravityFieldMesh->ignoreGridRaycasts = true;
	gravityFieldMesh->SetUVOffsetSpeed(XMFLOAT2(0.075f, 0.05f));
	gravityFieldMesh->SetUVRotationSpeed(0.05f);
	gravityFieldMesh->SetBlendState(BlendStates::Transparent);
	gravityFieldMesh->SetAlpha(0.5f);
	gravityFieldMesh->SetWorldScale(2.f);

}

void GravityCrystal::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	auto containedActors = gravityInfluenceTrigger->GetAllContainedActors<GridActor>();
	for (auto actor : containedActors)
	{
		if (actor->GetCanFall())
		{
			previouslyContainedActors.emplace(actor);

			actor->SetCanFall(false);

			const auto nextPos = actor->GetPositionV() + VMath::GlobalUpVector();
			actor->SetNextPos(nextPos);

			for (auto mesh : actor->GetComponents<MeshComponent>())
			{
				mesh->SetShaderItem("Floating");
			}

			actor->RecalcCurrentNodeDontIgnoreThis();
		}
	}

	for (auto actorIt = previouslyContainedActors.begin(); actorIt != previouslyContainedActors.end();)
	{
		auto actor = (*actorIt);

		if (!gravityInfluenceTrigger->Contains(actor->GetPositionV()))
		{
			actor->SetCanFall(true);

			const auto nextPos = actor->GetPositionV() - VMath::GlobalUpVector();
			actor->SetNextPos(nextPos);

			actor->RecalcCurrentNodeDontIgnoreThis();

			previouslyContainedActors.erase(actor);

			//This is fucking stupid. The std::set iterator crashes if you erase in the for loop like this.
			if (previouslyContainedActors.size() == 0)
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
