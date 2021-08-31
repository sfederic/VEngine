#include "NormalActor.h"
#include "Components/MeshComponent.h"
#include "Camera.h"
#include "VMath.h"
#include "Input.h"

NormalActor::NormalActor()
{
	mesh1 = MeshComponent::system.Add(this, MeshComponent("cube.fbx"));
	rootComponent = mesh1;

	camera = CameraComponent::system.Add(this, CameraComponent(XMFLOAT3(0.f, 5.f, -20.f), false));
	rootComponent->AddChild(camera);
}

void NormalActor::Tick(double deltaTime)
{
	//camera->Tick(deltaTime);

	if (Input::GetKeyUp(Keys::D))
	{
		auto rot = GetRotation();
		auto rotVec = XMLoadFloat4(&rot);

		float angle = DirectX::XMConvertToRadians(90.f);
		auto result = DirectX::XMQuaternionMultiply(rotVec, DirectX::XMQuaternionRotationAxis(VMath::XMVectorUp(), angle));
		SetRotation(result);
	}
}

Properties NormalActor::GetProps()
{
	return __super::GetProps();
}
