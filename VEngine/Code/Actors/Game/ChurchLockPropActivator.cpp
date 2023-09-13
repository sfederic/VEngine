#include "vpch.h"
#include "ChurchLockPropActivator.h"
#include "Components/MeshComponent.h"

void ChurchLockPropActivator::Activate()
{
	__super::Activate();

	mesh->SetAmbientColour(XMFLOAT4(0.9f, 0.7f, 0.5f, 1.f));
}