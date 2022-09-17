#include "vpch.h"
#include "Door.h"
#include "Components/MeshComponent.h"

Door::Door()
{
    mesh = CreateComponent(MeshComponent(), "Mesh");
    rootComponent = mesh;
}

Properties Door::GetProps()
{
    Properties props = __super::GetProps();
    props.Add("Open", &isOpen);
    return props;
}

void Door::Open()
{
    isOpen = true;
    mesh->active = false;
}
