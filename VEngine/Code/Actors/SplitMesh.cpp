#include "vpch.h"
#include "SplitMesh.h"
#include "Components/MeshComponent.h"
#include "Core/Timer.h"
#include "Render/Material.h"
#include "Render/RastStates.h"
#include "Render/Vertex.h"

SplitMesh::SplitMesh()
{
	mesh = CreateComponent<MeshComponent>("SlicedMesh");
	rootComponent = mesh;
}

void SplitMesh::Tick(float deltaTime)
{
	destroyTimer += deltaTime;
	if (destroyTimer > 3.f)
	{
		delete mesh->meshDataProxy.vertices;
		Destroy();
	}
}

void SplitMesh::CreateSplitMesh(std::vector<Vertex>& meshVerts, MeshComponent* originalMesh)
{
	mesh->SetRastState(RastStates::noBackCull);

	//Call 'new' here, can't just give the vertices a pointer to nothing
	mesh->meshDataProxy.vertices = new std::vector<Vertex>();
	*mesh->meshDataProxy.vertices = meshVerts;

	mesh->isStatic = false;

	auto originalMeshTransform = originalMesh->GetTransform();
	mesh->SetTransform(originalMeshTransform);

	mesh->SetTexture(originalMesh->GetTextureFilename());

	mesh->GetMaterial().materialShaderData = originalMesh->GetMaterial().materialShaderData;

	mesh->SplitMeshCreate();
}

Properties SplitMesh::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}
