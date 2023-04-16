#include "vpch.h"
#include "BreakableCube.h"
#include "Components/MeshComponent.h"
#include "Render/Material.h"
#include "Core/VMath.h"

void BreakableCube::Create()
{
	interactText = L"Looks breakable.";

	mesh->SetTexture("Walls/wall (2).png");

	const int indexRange = VMath::RandomRangeInt(0, 4);
	std::vector<float> angles{ 0.f, 90.f, 180.f, 270.f, 360.f };
	auto& mat = mesh->GetMaterial();
	mat.materialShaderData.uvRotation = angles[indexRange];
	mat.materialShaderData.uvScale = XMFLOAT2(3.f, 3.f);
}
