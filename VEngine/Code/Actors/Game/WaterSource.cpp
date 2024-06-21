#include "vpch.h"
#include "WaterSource.h"
#include "Components/BoxTriggerComponent.h"
#include "Components/MeshComponent.h"

WaterSource::WaterSource()
{
	boxTrigger = CreateComponent<BoxTriggerComponent>("BoxTrigger");
	AddChildToRoot(boxTrigger);
}

void WaterSource::Create()
{
	__super::Create();

	canBeMovedInLink = false;
	canBeRotatedInLink = false;

	_mesh->SetMeshFilename("cube.vmesh");
	_mesh->ignoreGridRaycasts = true;
	_mesh->SetVisibility(false);
}

void WaterSource::Start()
{
	__super::Start();
}

void WaterSource::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	DouseGridActorsInTrigger();
}

Properties WaterSource::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}

void WaterSource::Activate()
{
	__super::Activate();

	SetVisibility(true);
}

void WaterSource::Deactivate()
{
	__super::Deactivate();

	SetVisibility(false);
}

bool WaterSource::Contains(XMVECTOR point)
{
	return boxTrigger->Contains(point);
}

void WaterSource::DouseGridActorsInTrigger()
{
	for (auto gridActor : World::GetAllActorsAsBaseType<GridActor>())
	{
		if (boxTrigger->Contains(gridActor->GetPositionV()))
		{
			if (gridActor->CheckMovementAndRotationStopped())
			{
				gridActor->Douse();
			}
		}
	}
}
