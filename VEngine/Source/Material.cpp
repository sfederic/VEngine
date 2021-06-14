#include "Material.h"
#include "Serialise.h"

Material* MaterialSystem::CreateMaterialFromFile(const std::string& filename)
{
	Material newMaterial = {};

	std::filebuf fb;
	fb.open(filename.c_str(), std::ios_base::in);
	std::istream is(&fb);

	materials.push_back(newMaterial);
	return &materials.back();
}

Properties Material::GetProps()
{
	Properties props;
	props.Add("Name", &name);
	props.Add("Colour", &colour);
	return props;
}

void Material::SaveToFile()
{
	std::string filename = "Materials/" + name;
	Serialiser s(filename, std::ios_base::out);

	std::ostream os(&s.fb);
	Serialiser::Serialise(GetProps(), os);
}
