#include "MeshComponent.h"
#include "FBXImporter.h"
#include "Render/Renderer.h"

void MeshComponent::Create()
{
	//Import mesh
	fbxImporter.Import(filename.c_str(), data);

	//Setup bounds
	BoundingOrientedBox::CreateFromPoints(boundingBox, data.vertices.size(), &data.vertices[0].pos, sizeof(Vertex));

	//Setup pipeline objects
	pso.vertexBuffer.data = renderer.CreateVertexBuffer(data);
	pso.indexBuffer.data = renderer.CreateIndexBuffer(data);
	pso.sampler.data = renderer.CreateSampler();
}
