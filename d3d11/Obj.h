#pragma once

#include "RenderSystem.h"
#include <vector>
#include <DirectXMath.h>

using namespace DirectX;

struct OBJData
{
	UINT GetByteWidth()
	{
		return (UINT)(sizeof(Vertex) * verts.size());
	}

	std::vector<Vertex> verts;
};

bool LoadOBJFile(const char* filename, OBJData& data);
