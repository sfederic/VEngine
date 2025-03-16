
#include "SplitMesh.h"
import Components.MeshComponent;
#include "Render/Material.h"
#include "Render/RastStates.h"
#include "Render/Vertex.h"

SplitMesh::SplitMesh()
{
	mesh = CreateComponent<MeshComponent>("SlicedMesh");
	SetRootComponent(mesh);
}

void SplitMesh::Tick(float deltaTime)
{
	destroyTimer += deltaTime;
	if (destroyTimer > 3.f)
	{
		Remove();
	}
}

void SplitMesh::CreateSplitMesh(std::vector<Vertex>& meshVerts, MeshComponent* originalMesh)
{
	mesh->SetRastState(RastStates::noBackCull);

	//Call 'new' here, can't just give the vertices a pointer to nothing
	mesh->meshDataProxy.vertices = meshVerts;

	mesh->SetPhysicsStatic(false);

	auto originalMeshTransform = originalMesh->GetTransform();
	mesh->SetTransform(originalMeshTransform);

	mesh->SetTexture(originalMesh->GetTextureFilename());

	mesh->GetMaterial().GetMaterialShaderData() = originalMesh->GetMaterial().GetMaterialShaderData();

	mesh->SplitMeshCreate();
}

Properties SplitMesh::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}
