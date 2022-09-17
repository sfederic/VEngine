#include "vpch.h"
#include "PhotoActor.h"
#include "Components/Game/PhotoComponent.h"
#include "Components/MeshComponent.h"

PhotoActor::PhotoActor()
{
	photoComponent = CreateComponent(PhotoComponent(), "Photo");

	meshComponent = CreateComponent(MeshComponent(), "Mesh");
	rootComponent = meshComponent;
}

Properties PhotoActor::GetProps()
{
	return __super::GetProps();
}
