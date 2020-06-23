#pragma once

#include <vector>

struct OBJData
{
	std::vector<float> verts;
	std::vector<float> uvs;
	std::vector<float> normals;
};

bool loadOBJFile(const char* filename, OBJData& data);
