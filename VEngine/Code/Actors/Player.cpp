#include "Player.h"
#include "Components/MeshComponent.h"
#include "Camera.h"

Player::Player()
{
	mesh = MeshComponent::system.Add(this, MeshComponent("cube.fbx", "wall.png"));
	rootComponent = mesh;

	camera = CameraComponent::system.Add(this, CameraComponent(XMFLOAT3(0.f, 5.f, -20.f), false));
	rootComponent->AddChild(camera);
}

void Player::Start()
{
}

void Player::Tick(double deltaTime)
{
}

Properties Player::GetProps()
{
    return Actor::GetProps();
}
