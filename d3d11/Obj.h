#pragma once

#include <vector>
#include <DirectXMath.h>

using namespace DirectX;

struct OBJData
{
	std::vector<XMFLOAT3> verts;
	std::vector<XMFLOAT2> uvs;
	std::vector<XMFLOAT3> normals;
};

bool loadOBJFile(const char* filename, OBJData& data);
