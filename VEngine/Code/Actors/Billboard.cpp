#include "Billboard.h"
#include "Components/MeshComponent.h"
#include "Render/Material.h"
#include "Camera.h"

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

    XMFLOAT3 pos = GetPosition();

    const float angle = atan2(activeCamera->transform.world.r[3].m128_f32[0] - pos.x,
        activeCamera->transform.world.r[3].m128_f32[2] - pos.z) * (180.0 / XM_PI);

    const float rotation = XMConvertToRadians(angle);

    XMMATRIX m = XMMatrixRotationY(rotation);
    XMVECTOR rot = XMQuaternionRotationMatrix(m);
    SetRotation(rot);

    XMVECTOR posv = GetPositionVector();
    posv += velocity;
    SetPosition(posv);
}

Properties Billboard::GetProps()
{
    return Actor::GetProps();
}
