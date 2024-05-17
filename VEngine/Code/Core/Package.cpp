#include "vpch.h"
#include <filesystem>

//@Todo: testing code for packaging. Liked it enough to put it here from testing. see how it goes.

struct Resource
{
	const static inline int elementSize = sizeof(uint8_t);
	uint8_t* data = nullptr;
	uint64_t size = 0;
};

struct ResourceDataEntry
{
	const static inline int NAME_MAX = 64;
	char name[NAME_MAX] = {};
	uint64_t offset = 0;
	uint64_t size = 0;
};

struct Package
{
	std::vector<Resource> resources;
};

std::map<std::string, Resource> gTextures;
std::map<std::string, ResourceDataEntry> gEntries;

Resource ReadFile(const std::string& filename)
{
	FILE* file = nullptr;
	fopen_s(&file, filename.c_str(), "rb");
	assert(file);
	fseek(file, 0, SEEK_END);
	uint64_t size = ftell(file);
	rewind(file);
	uint8_t* data = (uint8_t*)malloc(size);
	assert(data);
	fread(data, 1, size, file);
	fclose(file);
	return Resource{ data, size };
}

void GetAllFilesOfExtension()
{
	for (auto& path : std::filesystem::directory_iterator(std::filesystem::current_path()))
	{
		if (path.path().filename().extension() == ".png")
		{
			std::string file = path.path().filename().string();
			gTextures[file] = ReadFile(file);
		}
	}
}

void WriteToPackage()
{
	FILE* file = nullptr;
	fopen_s(&file, "textures.pkg", "wb");
	assert(file);

	size_t pSize = gTextures.size();
	fwrite(&pSize, sizeof(size_t), 1, file); //Num of ResourceEntries

	std::vector<ResourceDataEntry> entries;

	for (auto& texturePair : gTextures)
	{
		auto& resource = texturePair.second;

		ResourceDataEntry entry = {};
		assert(texturePair.first.length() < ResourceDataEntry::NAME_MAX);
		strcpy_s(entry.name, texturePair.first.c_str());
		entry.offset = ftell(file);
		entry.size = resource.size;
		entries.emplace_back(entry);

		fwrite(resource.data, Resource::elementSize, resource.size, file);
	}

	//Write table of contents to EOF
	for (auto& entry : entries)
	{
		fwrite(&entry, sizeof(ResourceDataEntry), 1, file);
	}

	fclose(file);
}

void ReadFromPackage()
{
	FILE* file = nullptr;
	fopen_s(&file, "textures.pkg", "rb");
	assert(file);

	size_t numEntries = 0; //TOC will be SEEK_END - (numEntries * sizeof(ResourceDataEntry))
	fread(&numEntries, sizeof(size_t), 1, file);

	size_t offset = numEntries * sizeof(ResourceDataEntry);
	fseek(file, static_cast<long>(offset), SEEK_END);

	for (size_t i = 0; i < numEntries; i++)
	{
		ResourceDataEntry entry = {};
		fread(&entry, sizeof(ResourceDataEntry), 1, file);
		gEntries[entry.name] = (entry);
	}

	fclose(file);
}

uint8_t* FindResouce(const std::string name)
{
	auto& entry = gEntries[name];

	FILE* file = nullptr;
	fopen_s(&file, "textures.pkg", "rb");
	assert(file);

	fseek(file, entry.offset, SEEK_SET);

	uint8_t* data = (uint8_t*)malloc(entry.size);
	assert(data);

	fread(data, sizeof(uint8_t), entry.size, file);
	fclose(file);

	return data;
}
