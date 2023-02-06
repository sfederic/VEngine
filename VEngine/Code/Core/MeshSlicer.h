#pragma once

#include <vector>
#include "Render/RenderTypes.h"

class MeshComponent;
struct Transform;

namespace MeshSlicer
{
	void SliceMeshViaPlane(XMVECTOR planeCenter,
		XMVECTOR planeNormal,
		MeshComponent& mesh,
		std::vector<Vertex>& mesh0Verts,
		std::vector<Vertex>& mesh1Verts);

	void CreateSlicedMesh(const std::vector<Vertex>& meshVerts, const Transform& originalMeshTrasnform);
};
