export module Render.MeshData;

#include <DirectXCollision.h>
#include "Animation/Skeleton.h"
#include "Vertex.h"

//The actual data for each loaded mesh. Each loaded mesh file will have one of these per its filename.
export struct MeshData
{
	typedef uint32_t indexDataType;

	//Base extents and offset will be the same for each mesh, fine to cache here.
	DirectX::BoundingBox boundingBox;

	std::vector<Vertex> vertices;

	Skeleton skeleton;
};
