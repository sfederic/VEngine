#pragma once

import <vector>;
#include "Render/Vertex.h"

class MeshComponent;

namespace MeshSlicer
{
	void SliceMeshViaPlane(DirectX::XMVECTOR planeCenter,
		DirectX::XMVECTOR planeNormal,
		MeshComponent& mesh,
		std::vector<Vertex>& mesh0Verts,
		std::vector<Vertex>& mesh1Verts);
};
