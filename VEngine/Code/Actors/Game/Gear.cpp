#include "vpch.h"
#include "Gear.h"
#include "Components/MeshComponent.h"

void Gear::Create()
{
	mesh->SetMeshFilename("gear.vmesh");
}

void Gear::OnLinkRotate()
{

}
