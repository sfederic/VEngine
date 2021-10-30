#include "AssetSystem.h"
#include <cstdio>
#include "FBXImporter.h"
#include "MeshAssetHeader.h"

extern AssetSystem assetSystem;

void AssetSystem::WriteAllMeshDataToMeshAssetFiles()
{
	FILE* file = nullptr;

	for (auto meshIt : fbxImporter.existingMeshDataMap)
	{
		MeshData* meshData = meshIt.second;

		MeshAssetHeader header = {};
		header.sourceMeshFormat = SourceMeshFormat::FBX;
		header.indexCount = meshData->indices.size();
		header.vertexCount = meshData->vertices.size();

		const std::string& filename = meshIt.first;
		const std::string meshName = filename.substr(0, filename.find("."));
		const std::string meshFilename = meshName + ".vmesh";

		fopen_s(&file, meshFilename.c_str(), "wb");
		assert(file);

		fwrite(&header, sizeof(MeshAssetHeader), 1, file);
		fwrite(meshData->vertices.data(), sizeof(Vertex), meshData->vertices.size(), file);
		fwrite(meshData->indices.data(), sizeof(MeshData::indexDataType), meshData->indices.size(), file);

		fclose(file);
	}
}
