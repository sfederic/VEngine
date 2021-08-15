#include "MeshComponent.h"
#include "FBXImporter.h"
#include "Render/Renderer.h"

void MeshComponent::Create()
{
	if (system.systemState == SystemStates::Loaded)
	{
		filename = createFilename;
	}

	fbxImporter.Import(filename.c_str(), data);

	pso.vertexBuffer.data = renderer.CreateVertexBuffer(data);
	pso.indexBuffer.data = renderer.CreateIndexBuffer(data);
	pso.sampler.data = renderer.CreateSampler();
}
