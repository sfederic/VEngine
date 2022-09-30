#include "vpch.h"
#include "SkillNode.h"
#include "Components/MeshComponent.h"
#include "Render/Material.h"

SkillNode::SkillNode()
{
	mesh = CreateComponent(MeshComponent("node.fbx", "Particle/orange_gradient.png"), "Mesh");
	rootComponent = mesh;

	mesh->SetBlendState(BlendStates::Default);
	Material* material = mesh->GetMaterial();
	material->materialShaderData.ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 0.5f);
	material->uvOffsetSpeed = XMFLOAT2(0.5f, 0.5f);
}

Properties SkillNode::GetProps()
{
	return __super::GetProps();
}
