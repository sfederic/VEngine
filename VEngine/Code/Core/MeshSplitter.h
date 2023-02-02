#pragma once

#include <vector>
#include "Render/RenderTypes.h"

class MeshComponent;

namespace MeshSplitter
{
	void SplitMeshViaPlane(MeshComponent& mesh,
		std::vector<Vertex>& mesh0Verts,
		std::vector<Vertex>& mesh1Verts);
};
