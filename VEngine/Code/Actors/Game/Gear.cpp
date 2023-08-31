#include "vpch.h"
#include "Gear.h"
#include "GearSet.h"
#include "Components/MeshComponent.h"

void Gear::Create()
{
	ignoreRotationValidCheck = true;

	SetMeshFilename("gear.vmesh");
}

void Gear::OnLinkRotate()
{
	__super::OnLinkRotate();

	//This is so any adjacent gears will cascade their movement, think like a watch's internals.
	//Code here is a bit complex, but it works.
	auto& gears = Gear::system.GetActors();
	for (int gearIndex = 0; gearIndex < gears.size(); gearIndex++)
	{
		auto gear = gears[gearIndex].get();

		if (gear == this)
		{
			continue;
		}

		if (CheckIfPreviousGear(gear))
		{
			continue;
		}

		const auto bounds = this->GetRootComponent().GetBoundsInWorldSpace();
		if (gear->Intersects(bounds))
		{
			gear->previouslyCascadedGears.push_back(this);
			this->previouslyCascadedGears.push_back(gear);

			gear->OnLinkRotate();
			gear->AddNextRotation(gear->GetUpVectorV(), 90.f);
		}
	}

	for (auto& gearSet : GearSet::system.GetActors())
	{
		if (gearSet->Intersects(mesh->GetBoundsInWorldSpace()))
		{
			gearSet->AddNextRotation(gearSet->GetUpVectorV(), 90.f);
			gearSet->IncrementDoor();
		}
	}
}

void Gear::OnRotationEnd()
{
	previouslyCascadedGears.clear();
}

bool Gear::Intersects(const BoundingOrientedBox& bob)
{
	return rootComponent->GetBoundsInWorldSpace().Intersects(bob);
}

bool Gear::CheckIfPreviousGear(Gear* gear)
{
	for (auto previousGear : previouslyCascadedGears)
	{
		if (gear == previousGear)
		{
			return true;
		}
	}

	return false;
}
