#include "vpch.h"
#include "BuildingRubble.h"
#include "Components/MeshComponent.h"
#include "Core/Core.h"

BuildingRubble::BuildingRubble()
{
    mesh = CreateComponent("Mesh", MeshComponent("broken_building.vmesh", "destroyed_building.jpg"));
    rootComponent = mesh;
}

void BuildingRubble::Tick(float deltaTime)
{
    //Bob rubble up and down
    XMFLOAT3 pos = GetPosition();
    pos.y = std::cosf(Core::timeSinceStartup) * deltaTime;
    SetPosition(pos);
}

Properties BuildingRubble::GetProps()
{
    auto props = __super::GetProps();
    props.title = GetTypeName();
    return props;
}
