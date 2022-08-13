#include "vpch.h"
#include "AssetSystem.h"
#include <cstdio>
#include <filesystem>
#include "FBXLoader.h"
#include "MeshAssetHeader.h"
#include "Profile.h"
#include "Log.h"
#include "FileSystem.h"
#include "Asset/AssetPaths.h"

const int ANIM_NAME_SIZE = 64;

AssetSystem assetSystem;

AssetSystem::AssetSystem() : System("AssetSystem")
{
}

void AssetSystem::WriteAllMeshDataToMeshAssetFiles()
{
	FILE* file = nullptr;

	uint32_t numberOfMeshFilesBuilt = 0;

	auto startTime = Profile::QuickStart();

	for (auto& meshIt : FBXLoader::existingMeshDataMap)
	{
		MeshData& meshData = meshIt.second;

		MeshAssetHeader header = {};
		header.sourceMeshFormat = SourceMeshFormat::FBX;
		header.indexCount = meshData.indices.size();
		header.vertexCount = meshData.vertices.size();
		header.boneCount = meshData.skeleton.joints.size();

		const std::string& filename = meshIt.first;
		const std::string meshName = filename.substr(0, filename.find("."));
		const std::string meshFilePath = AssetBaseFolders::mesh + meshName + ".vmesh";

		fopen_s(&file, meshFilePath.c_str(), "wb");
		assert(file);

		assert(fwrite(&header, sizeof(MeshAssetHeader), 1, file));
		assert(fwrite(meshData.vertices.data(), sizeof(Vertex), meshData.vertices.size(), file));
		assert(fwrite(meshData.indices.data(), sizeof(MeshData::indexDataType), meshData.indices.size(), file));
		assert(fwrite(&meshData.boudingBox, sizeof(DirectX::BoundingBox), 1, file));

		const auto& joints = meshData.skeleton.joints;
		assert(fwrite(joints.data(), sizeof(Joint), joints.size(), file));

		//Num animations
		const size_t numAnimations = meshData.skeleton.animations.size();
		assert(fwrite(&numAnimations, sizeof(size_t), 1, file));

		for (const auto& animationPair : meshData.skeleton.animations)
		{
			//Animation name
			const std::string& animationName = animationPair.first;
			char animationNameBuff[64]{};
			strcpy_s(animationNameBuff, ANIM_NAME_SIZE, animationName.c_str());
			assert(fwrite(animationNameBuff, sizeof(char), ANIM_NAME_SIZE, file));

			//Num frames
			const size_t numAnimFrames = animationPair.second.frames.size();
			assert(fwrite(&numAnimFrames, sizeof(numAnimFrames), 1, file));

			for (const auto& framePair : animationPair.second.frames)
			{
				const size_t numFrames = framePair.second.size();
				assert(fwrite(&numFrames, sizeof(numFrames), 1, file));

				for (const auto& frame : framePair.second)
				{
					//Anim Frames
					const int jointIndex = framePair.first;
					assert(fwrite(&jointIndex, sizeof(int), 1, file));

					assert(fwrite(&frame, sizeof(AnimFrame), 1, file));
				}
			}
		}

		fclose(file);

		numberOfMeshFilesBuilt++;
	}

	double elapsedTime = Profile::QuickEnd(startTime);

	Log("Mesh asset build complete.\n\tNum meshes built: %d\n\tTime taken: %f", numberOfMeshFilesBuilt, elapsedTime);
}

//@Todo: This is just testing code. Get rid of it or replace.
MeshData AssetSystem::ReadAllMeshAssetsFromFile(const char* filename)
{
	FILE* file = nullptr;
	std::string filepath = "Meshes/" + std::string(filename);
	fopen_s(&file, filepath.c_str(), "rb");
	assert(file);

	MeshAssetHeader header;
	MeshData data;

	assert(fread(&header, sizeof(MeshAssetHeader), 1, file));

	data.vertices.resize(header.vertexCount);
	data.indices.resize(header.indexCount);

	assert(fread(data.vertices.data(), sizeof(Vertex), header.vertexCount, file));
	assert(fread(data.indices.data(), sizeof(MeshData::indexDataType), header.indexCount, file));
	assert(fread(&data.boudingBox, sizeof(DirectX::BoundingBox), 1, file));

	data.skeleton.joints.resize(header.boneCount);
	assert(fread(data.skeleton.joints.data(), sizeof(Joint), header.boneCount, file));

	//Num animations
	size_t numAnimations = 0;
	fread(&numAnimations, sizeof(size_t), 1, file);

	for(size_t animIndex = 0; animIndex < numAnimations; animIndex++)
	{
		char animationNameBuff[ANIM_NAME_SIZE]{};
		assert(fread(animationNameBuff, sizeof(char), ANIM_NAME_SIZE, file));
		std::string animationName(animationNameBuff);
		data.skeleton.CreateAnimation(animationName);

		size_t numAnimFrames = 0;
		assert(fread(&numAnimFrames, sizeof(numAnimFrames), 1, file));

		for (size_t animFrameIndex = 0; animFrameIndex < numAnimFrames; animFrameIndex++)
		{
			size_t numFrames = 0;
			assert(fread(&numFrames, sizeof(numFrames), 1, file));

			for (size_t frameIndex = 0; frameIndex < numFrames; frameIndex++)
			{
				int jointIndex = -2; //Parent index is -1, so -2 will mean the read errored
				assert(fread(&jointIndex, sizeof(int), 1, file));

				AnimFrame frame{};
				assert(fread(&frame, sizeof(AnimFrame), 1, file));

				Animation& animation = data.skeleton.animations.find(animationName)->second;
				animation.frames[jointIndex].push_back(frame);
			}
		}
	}

	return data;
}

void AssetSystem::BuildAllGameplayMapFiles()
{
	for (auto const& dirEntry : std::filesystem::recursive_directory_iterator{ "WorldMaps" })
	{
		std::string file = dirEntry.path().filename().string();
		FileSystem::CreateGameplayWorldSave(file);
	}
}
