#pragma once

#include <vector>
#include "DXUtil.h"
#include <DirectXMath.h>

using namespace DirectX;

struct OBJData
{
	std::vector<Vertex> verts;

	//std::vector<XMFLOAT3> verts;
	//std::vector<XMFLOAT2> uvs;
	//std::vector<XMFLOAT3> normals;
};

bool loadOBJFile(const char* filename, OBJData& data);
