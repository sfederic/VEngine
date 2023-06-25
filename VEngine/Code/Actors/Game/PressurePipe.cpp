#include "vpch.h"
#include "PressurePipe.h"
#include "Components/MeshComponent.h"

PressurePipe::PressurePipe()
{
	flowMesh = CreateComponent<MeshComponent>("FlowMesh");
	rootComponent->AddChild(flowMesh);
}

void PressurePipe::Create()
{
	flowMesh->SetMeshFilename("cylinder.vmesh");
	flowMesh->SetTexture("water.jpg");
	flowMesh->SetLocalScale(0.5f);
	flowMesh->SetLocalPosition(XMVectorSet(0.f, 1.f, 0.f, 1.f));
	flowMesh->SetUVOffsetSpeed(XMFLOAT2(0.f, -0.4f));
}
