#include "vpch.h"
#include "OilBarrel.h"
#include "Core/VMath.h"
#include "Physics/Raycast.h"
#include "OilSlick.h"
#include "Components/MeshComponent.h"

void OilBarrel::Create()
{
	__super::Create();

	ignoreRotationValidCheck = true;

	SetMeshFilename("oil_barrel.vmesh");
}

void OilBarrel::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	if (CheckMovementAndRotationStopped())
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
	if (!VMath::VecEqual(GetUpVectorV(), VMath::GlobalUpVector()))
	{
		HitResult hit(this);
		hit.IgnorePlayer();

		const XMVECTOR raycastOrigin = GetPositionV() + GetUpVectorV();
		if (Physics::Raycast(hit, raycastOrigin, -VMath::GlobalUpVector(), 100.f))
		{
			Transform t;
			t.position = hit.hitPos;
			t.position.y += 0.05f;

			//Don't spawn oil slicks on top of each other
			if (hit.GetHitActorAs<OilSlick>() != nullptr)
			{
				return;
			}

			auto oilSlick = OilSlick::system.Add(t);
			oilSlick->UpdateTransform(XMMatrixIdentity());
			oilSlick->Create();
			oilSlick->CreateAllComponents();

			const float randYAxisAngle = VMath::RandomRange(0.f, 360.f);
			oilSlick->GetMesh().AddLocalRotation(VMath::GlobalUpVector(), randYAxisAngle);

			//Remember to call start else nextPos and nextRot setup will get skipped.
			oilSlick->Start();

			spillOilPerNode = false;
		}
	}
}
