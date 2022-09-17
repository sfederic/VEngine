#include "vpch.h"
#include "PlayerShip.h"
#include "Input.h"
#include "Components/MeshComponent.h"
#include "Components/CameraComponent.h"

PlayerShip::PlayerShip()
{
    rootComponent = CreateComponent(MeshComponent(), "Mesh");

    camera = CreateComponent(CameraComponent(), "Camera");
    rootComponent->AddChild(camera);
    camera->SetPosition(0.f, 2.f, -5.f);
    camera->targetActor = this;
}

void PlayerShip::Start()
{
    camera->targetActor = this;
}

void PlayerShip::Tick(float deltaTime)
{
    MovementInput(deltaTime);
}

Properties PlayerShip::GetProps()
{
    return __super::GetProps();
}

void PlayerShip::MovementInput(float deltaTime)
{
    if (Input::GetKeyHeld(Keys::W))
    {
        SetPosition(GetPositionV() + (GetForwardVectorV() * moveSpeed * deltaTime));
    }
    else if (Input::GetKeyHeld(Keys::S))
    {
        SetPosition(GetPositionV() - (GetForwardVectorV() * moveSpeed * deltaTime));
    }

    if (Input::GetKeyHeld(Keys::A))
    {
        const XMMATRIX r = XMMatrixRotationY(deltaTime * -rotateSpeed);
        const XMVECTOR q = XMQuaternionMultiply(GetRotationV(), XMQuaternionRotationMatrix(r));
        SetRotation(q);
    }
    else if (Input::GetKeyHeld(Keys::D))
    {
        const XMMATRIX r = XMMatrixRotationY(deltaTime * rotateSpeed);
        const XMVECTOR q = XMQuaternionMultiply(GetRotationV(), XMQuaternionRotationMatrix(r));
        SetRotation(q);
    }
}
