#include "Material.h"
#include <fstream>

Material* MaterialSystem::CreateMaterialFromFile(const std::string& filename)
{
	Material newMaterial = {};

	std::filebuf fb;
	fb.open(filename.c_str(), std::ios_base::in);
	std::istream is(&fb);

	materials.push_back(newMaterial);
	return &materials.back();
}
