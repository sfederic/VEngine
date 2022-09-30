#include "vpch.h"
#include "AOELine.h"
#include "Components/MeshComponent.h"
#include "Render/Material.h"

AOELine::AOELine()
{
	mesh = CreateComponent(MeshComponent("node.fbx", "Particle/orange_gradient.png"), "Mesh");
	rootComponent = mesh;

	mesh->SetBlendState(BlendStates::Default);
	Material* material = mesh->GetMaterial();
	material->materialShaderData.ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 0.5f);
	material->uvOffsetSpeed = XMFLOAT2(0.5f, 0.5f);
}

Properties AOELine::GetProps()
{
	return __super::GetProps();
}
