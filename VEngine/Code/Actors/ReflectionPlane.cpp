#include "vpch.h"
#include "ReflectionPlane.h"
#include "Camera.h"
#include "VMath.h"
#include "Components/MeshComponent.h"
#include "Gameplay/GameUtils.h"

ReflectionPlane::ReflectionPlane()
{
    reflectDirection = XMFLOAT3(0.f, 1.f, 0.f);

    //Keep this camera out of the component hierarchy
    reflectionCamera = CameraComponent::system.Add(this, CameraComponent(XMFLOAT3(0.f, 0.f, 0.f), false));

    planeMesh = MeshComponent::system.Add(this, MeshComponent("plane.fbx", "test.png", "PlanarReflection.hlsl"));
    rootComponent = planeMesh;
}

void ReflectionPlane::Start()
{
}

void ReflectionPlane::Tick(float deltaTime)
{
    auto activeCameraPos = activeCamera->GetWorldPositionV();
    auto planePos = GetPositionVector();
    auto reflectionCameraPos = reflectionCamera->GetWorldPositionV();

    //testing on x-plane only
    XMVECTOR newPos = activeCameraPos;
    float diff =  planePos.m128_f32[0] - activeCameraPos.m128_f32[0];
    newPos.m128_f32[0] = planePos.m128_f32[0] + diff;

    reflectionCamera->SetPosition(newPos);
}

Properties ReflectionPlane::GetProps()
{
    return __super::GetProps();
}

XMMATRIX ReflectionPlane::GetReflectionViewMatrix()
{
    auto player = (Actor*)GameUtils::GetPlayer();
    XMMATRIX view = XMMatrixLookAtLH(reflectionCamera->GetPositionV(),
        player->GetPositionVector(), VMath::XMVectorUp());
    return view;
}
