#include "Billboard.h"
#include "Components/MeshComponent.h"
#include "Render/Material.h"
#include "Camera.h"
#include "VMath.h"

Billboard::Billboard()
{
    mesh = MeshComponent::system.Add(this, MeshComponent("plane.fbx", "bush.png", "Unlit.hlsl"));
    mesh->material->rastStateName = "nobackcull";
    rootComponent = mesh;

    velocity = XMVectorSet(0.f, 0.f, 0.f, 1.f);
}

void Billboard::Tick(float deltaTime)
{
    lifeTime += deltaTime;
    if (lifeTime > 2.0f)
    {
        Destroy();
        return;
    }

    VMath::RotateTowardsCamera(rootComponent->transform);

    XMVECTOR posv = GetPositionVector();
    posv += velocity;
    SetPosition(posv);
}

Properties Billboard::GetProps()
{
    return Actor::GetProps();
}
