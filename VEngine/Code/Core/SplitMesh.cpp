#include "vpch.h"
#include "SplitMesh.h"
#include "Components/MeshComponent.h"
#include "Core/Timer.h"

void SplitMesh::Create(std::vector<Vertex>& meshVerts, const Transform& originalMeshTransform)
{
	auto mesh = MeshComponent::system.Add("SlicedMesh", nullptr, MeshComponent(), false);
	mesh->SetRastState(RastStates::noBackCull);

	//Call 'new' here, can't just give the vertices a pointer to nothing
	mesh->meshDataProxy.vertices = new std::vector<Vertex>();
	*mesh->meshDataProxy.vertices = meshVerts;

	mesh->isStatic = false;
	mesh->transform = originalMeshTransform;

	mesh->SplitMeshCreate();

	Timer::SetTimer(3.f, std::bind(&SplitMesh::Destroy, this));
}

void SplitMesh::Destroy()
{
	delete mesh->meshDataProxy.vertices;
	mesh->Remove();
}
