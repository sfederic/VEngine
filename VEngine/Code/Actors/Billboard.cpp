#include "Billboard.h"
#include "Components/MeshComponent.h"
#include "Render/Material.h"
#include "Camera.h"

Billboard::Billboard()
{
    mesh = MeshComponent::system.Add(this, MeshComponent("plane.fbx", "bush.png"));
    mesh->material->shaderFilename = "Unlit.hlsl";
    rootComponent = mesh;
}

void Billboard::Tick(double deltaTime)
{
    XMFLOAT3 pos = GetPosition();

    //REF:http://www.rastertek.com/dx11tut34.html
    float angle = atan2(activeCamera->transform.world.r[3].m128_f32[0] - pos.x,
        activeCamera->transform.world.r[3].m128_f32[2] - pos.z) * (180.0 / XM_PI);

    float rotation = XMConvertToRadians(angle);

    XMMATRIX m = XMMatrixRotationY(rotation);
    XMVECTOR rot = XMQuaternionRotationMatrix(m);
    SetRotation(rot);
}

Properties Billboard::GetProps()
{
    return Actor::GetProps();
}
