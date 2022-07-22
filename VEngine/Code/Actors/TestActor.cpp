#include "vpch.h"
#include "TestActor.h"
#include "Components/MeshComponent.h"

TestActor::TestActor()
{
    mesh1 = MeshComponent::system.Add(this, MeshComponent("cube.fbx", "test.png"), "Mesh1");
    rootComponent = mesh1;

    mesh2 = MeshComponent::system.Add(this, MeshComponent("cube.fbx", "test.png"), "Mesh2");
    mesh1->AddChild(mesh2);
    mesh2->SetPosition(0.f, 3.f, 0.f);
}

void TestActor::Start()
{
}

Properties TestActor::GetProps()
{
    return __super::GetProps();
}
