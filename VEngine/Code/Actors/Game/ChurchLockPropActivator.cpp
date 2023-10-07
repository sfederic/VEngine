#include "vpch.h"
#include "ChurchLockPropActivator.h"
#include "Components/MeshComponent.h"

void ChurchLockPropActivator::Activate()
{
	__super::Activate();

	mesh->SetAmbientColour(XMFLOAT3(0.9f, 0.7f, 0.5f));
}
