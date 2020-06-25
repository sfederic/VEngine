#pragma once

#include <vector>
#include "DXUtil.h"
#include <DirectXMath.h>

using namespace DirectX;

struct OBJData
{
	std::vector<Vertex> verts;

	UINT GetByteWidth()
	{
		return (UINT)(sizeof(Vertex) * verts.size());
	}
};

/*struct OBJDataRealloc
{
	Vertex* verts;
};*/

bool loadOBJFile(const char* filename, OBJData& data);
//bool loadOBJFileRealloc(const char* filename, OBJDataRealloc* data);
