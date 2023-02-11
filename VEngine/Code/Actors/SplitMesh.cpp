#include "vpch.h"
#include "SplitMesh.h"
#include "Components/MeshComponent.h"
#include "Core/Timer.h"

SplitMesh::SplitMesh()
{
	mesh = CreateComponent(MeshComponent(), "SlicedMesh");
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
	mesh->transform = originalMesh->transform;

	mesh->SetTexture(originalMesh->GetTextureFilename());

	mesh->SplitMeshCreate();
}

Properties SplitMesh::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}
