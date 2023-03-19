#pragma once

#include <DirectXCollision.h>
#include "Animation/Skeleton.h"
#include "Vertex.h"

//The actual data for each loaded mesh. Each loaded mesh file will have one of these per its filename.
struct MeshData
{
	typedef uint32_t indexDataType;

	std::vector<Vertex> vertices;

	//Base extents and offset will be the same for each mesh, fine to cache here.
	DirectX::BoundingBox boundingBox;

	//@Todo: Don't like skeleton being here, as it can then mean it's accessible from MeshComponents too.
	//Removing it means cleaning up FBXLoader functions a lot.
	Skeleton skeleton;
};
