#include "vpch.h"
#include "OilBarrel.h"
#include "Components/MeshComponent.h"
#include "Core/VMath.h"
#include "Physics/Raycast.h"

void OilBarrel::Create()
{
	ignoreRotationValidCheck = true;

	mesh->SetMeshFilename("barrel.vmesh");
}

void OilBarrel::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	if (HaveMovementAndRotationStopped())
	{
		if (spillOilPerNode)
		{
			SpillOil();
		}
	}
	else
	{
		spillOilPerNode = true;
	}
}

Properties OilBarrel::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}

void OilBarrel::SpillOil()
{
	if (VMath::VecEqual(GetUpVectorV(), -VMath::GlobalUpVector()))
	{
		HitResult hit(this);
		if (Raycast(hit, GetPositionV(), -VMath::GlobalUpVector(), 100.f))
		{
			Transform t;
			t.position = hit.hitPos;
			t.position.y += 0.05f;
			auto oilSlick = MeshComponent::system.Add("OilSlick", nullptr, MeshComponent("node.vmesh", "oil_slick.png"));
			oilSlick->Create();
			oilSlick->SetTransform(t);

			const float randYAxisAngle = VMath::RandomRange(0.f, 360.f);
			oilSlick->AddLocalRotation(VMath::GlobalUpVector(), randYAxisAngle);

			spillOilPerNode = false;
		}
	}
}
