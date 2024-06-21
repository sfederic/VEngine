#include "vpch.h"
#include "PressurePipe.h"
#include "Components/MeshComponent.h"

PressurePipe::PressurePipe()
{
	flowMesh = CreateComponent<MeshComponent>("FlowMesh");
	AddChildToRoot(flowMesh);
}

void PressurePipe::Create()
{
	__super::Create();

	flowMesh->SetMeshFilename("cylinder.vmesh");
	flowMesh->SetTexture("water.jpg");
	flowMesh->SetLocalScale(0.5f);
	flowMesh->SetLocalPosition(XMVectorSet(0.f, 1.f, 0.f, 1.f));
	flowMesh->SetUVOffsetSpeed(XMFLOAT2(0.f, -0.4f));
}

void PressurePipe::Start()
{
	__super::Start();

	if (!isActive)
	{
		flowMesh->SetActive(false);
		flowMesh->SetVisibility(false);
	}
}

Properties PressurePipe::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("IsActive", &isActive);
	return props;
}

void PressurePipe::Activate()
{
	__super::Activate();

	isActive = true;
	flowMesh->SetActive(true);
	flowMesh->SetVisibility(true);
}

void PressurePipe::Deactivate()
{
	__super::Deactivate();

	isActive = false;
	flowMesh->SetActive(false);
	flowMesh->SetVisibility(false);
}
