#include "vpch.h"
#include "Gear.h"
#include "GearSet.h"
#include "Components/MeshComponent.h"

using namespace DirectX;

void Gear::Create()
{
	__super::Create();
	ignoreRotationValidCheck = true;
	SetMeshFilename("gear.vmesh");
}

void Gear::OnLinkRotate()
{
	__super::OnLinkRotate();

	//This code is because gears don't normally cascade the same rotation, they'll invert
	//the rotation every second gear. Watch a video or something to see it in motion.
	const auto OddEvenAngleIncrement = [=](int index) -> float
		{
			const int oddIndex = index % 2;
			if (oddIndex > 0)
			{
				return -90.f;
			}
			else
			{
				return 90.f;
			}
		};

	//This is so any adjacent gears will cascade their movement, think like a watch's internals.
	//Code here is a bit complex, but it works.
	auto& gears = Gear::system.GetActors();
	for (int gearIndex = 0; gearIndex < gears.size(); gearIndex++)
	{
		auto gear = gears.at(gearIndex).get();

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
			gear->previouslyCascadedGears.emplace_back(this);
			this->previouslyCascadedGears.emplace_back(gear);

			gear->OnLinkRotate();

			const float angleIncrement = OddEvenAngleIncrement(gearIndex);
			gear->AddNextRotation(gear->GetUpVectorV(), angleIncrement);
		}
	}

	for (int gearIndex = 0; gearIndex < GearSet::system.GetNumActors(); gearIndex++)
	{
		auto& gearSet = GearSet::system.GetActors().at(gearIndex);
		if (gearSet->Intersects(mesh->GetBoundsInWorldSpace()))
		{
			const float angleIncrement = OddEvenAngleIncrement(gearIndex);
			gearSet->AddNextRotation(gearSet->GetUpVectorV(), angleIncrement);
			gearSet->IncrementDoor();
		}
	}
}

void Gear::OnRotationEnd()
{
	__super::OnRotationEnd();

	previouslyCascadedGears.clear();
}

Properties Gear::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}

bool Gear::Intersects(const DirectX::BoundingOrientedBox& bob)
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
