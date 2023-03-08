#include "vpch.h"
#include "Billboard.h"
#include "Components/MeshComponent.h"
#include "Core/VMath.h"
#include "Render/RastStates.h"

Billboard::Billboard()
{
    mesh = CreateComponent<MeshComponent>("Mesh");
    rootComponent = mesh;
}

void Billboard::Create()
{
    mesh->SetMeshFilename("plane.vmesh");
    mesh->SetRastState(RastStates::noBackCull);
    mesh->SetShaderFilenames(ShaderItems::Unlit);
}

void Billboard::Tick(float deltaTime)
{
    auto transform = GetTransform();
    VMath::RotateTowardsCamera(transform);
    SetTransform(transform);
}

Properties Billboard::GetProps()
{
    return __super::GetProps();
}
