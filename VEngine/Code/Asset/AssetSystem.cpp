#include "vpch.h"
#include "AssetSystem.h"
#include <cstdio>
#include <unordered_set>
#include <filesystem>
#include "FBXLoader.h"
#include "MeshAssetHeader.h"
#include "AnimationAssetHeader.h"
#include "Core/Profile.h"
#include "Core/Log.h"
#include "Core/FileSystem.h"
#include "Core/VString.h"
#include "Components/MeshComponent.h"
#include "Asset/AssetPaths.h"
#include "Asset/VertexColourData.h"
#include "Asset/VertexColourHeader.h"
#include "Render/MeshData.h"
#include "Render/Vertex.h"

std::map<std::string, MeshData> existingMeshData;

static const std::string vertexColourDataFileExtension = ".vertexcolourdata";

//@Todo: do something when importing all, remove the existing meshdata to match the meshes currently in world.
//That or make an offline process or a process when a file is added on a filewatcher's notice.
void AssetSystem::BuildAllVMeshDataFromFBXImport()
{
	FILE* file = nullptr;

	uint64_t numberOfMeshFilesBuilt = 0;

	auto startTime = Profile::QuickStart();

	std::unordered_set<std::string> fbxFilenames;

	//Import all FBX files
	for (const auto& entry : std::filesystem::directory_iterator("FBXFiles/"))
	{
		const std::string fbxFilename = entry.path().filename().string();
		fbxFilenames.emplace(fbxFilename);
	}

	for (auto& filename : fbxFilenames)
	{
		AssetSystem::BuildSingleVMeshFromFBX(filename);
		numberOfMeshFilesBuilt++;
	}

	double elapsedTime = Profile::QuickEnd(startTime);

	Log("Mesh asset build complete.\n\tNum meshes built: %d\n\tTime taken: %f",
		numberOfMeshFilesBuilt, elapsedTime);
}

void AssetSystem::BuildAllAnimationFilesFromFBXImport()
{
	FILE* file = nullptr;
	uint64_t numberOfAnimationFilesBuilt = 0;
	std::unordered_set<std::string> fbxAnimFilenames;

	auto startTime = Profile::QuickStart();

	for (const auto& entry : std::filesystem::directory_iterator("AnimationFBXFiles/"))
	{
		const std::string fbxFilename = entry.path().filename().string();
		fbxAnimFilenames.emplace(fbxFilename);
	}

	for (const auto& filename : fbxAnimFilenames)
	{
		AssetSystem::BuildSingleVAnimFromFBX(filename);
		numberOfAnimationFilesBuilt++;
	}

	double elapsedTime = Profile::QuickEnd(startTime);

	Log("Animation asset build complete.\nNum animations built: %d\n\tTime taken: %f",
		numberOfAnimationFilesBuilt, elapsedTime);
}

void AssetSystem::BuildSingleVMeshFromFBX(const std::string fbxFilename)
{
	MeshData meshData;
	FBXLoader::ImportAsMesh(fbxFilename, meshData);

	MeshAssetHeader header;
	header.sourceMeshFormat = SourceMeshFormat::FBX;
	header.vertexCount = meshData.vertices.size();
	header.boneCount = meshData.skeleton.GetNumJoints();

	const std::string meshName = fbxFilename.substr(0, fbxFilename.find("."));
	const std::string meshFilePath = AssetBaseFolders::mesh + meshName + ".vmesh";

	FILE* file = nullptr;
	fopen_s(&file, meshFilePath.c_str(), "wb");
	assert(file);

	assert(fwrite(&header, sizeof(MeshAssetHeader), 1, file));
	assert(fwrite(meshData.vertices.data(), sizeof(Vertex), meshData.vertices.size(), file));
	assert(fwrite(&meshData.boundingBox, sizeof(DirectX::BoundingBox), 1, file));

	auto& joints = meshData.skeleton.GetJoints();
	fwrite(joints.data(), sizeof(Joint), joints.size(), file);

	fclose(file);
}

void AssetSystem::BuildSingleVAnimFromFBX(const std::string filename)
{
	Animation animation = FBXLoader::ImportAsAnimation(filename);

	const std::string meshName = filename.substr(0, filename.find("."));
	const std::string meshFilePath = AssetBaseFolders::anim + meshName + ".vanim";

	FILE* file = nullptr;
	fopen_s(&file, meshFilePath.c_str(), "wb");
	assert(file);

	AnimationAssetHeader header;
	strcpy_s(header.name, sizeof(char) * Animation::ANIM_NAME_MAX, animation.GetName().c_str());
	header.frameCount = animation.GetFrames().size();
	assert(fwrite(&header, sizeof(AnimationAssetHeader), 1, file));

	for (auto& [jointIndex, animFrame] : animation.GetFrames())
	{
		assert(fwrite(&jointIndex, sizeof(int), 1, file));

		const size_t animFrameCount = animFrame.size();
		assert(fwrite(&animFrameCount, sizeof(size_t), 1, file));
		assert(fwrite(animFrame.data(), sizeof(AnimFrame), animFrameCount, file));
	}

	fclose(file);
}

MeshDataProxy AssetSystem::ReadVMeshAssetFromFile(const std::string filename)
{
	std::string filepath = AssetBaseFolders::mesh + filename;

	//Create VMesh if it doesn't exist yet.
	if (!std::filesystem::exists(filepath))
	{
		std::string fbxFile = VString::ReplaceFileExtesnion(filename, ".fbx");
		BuildSingleVMeshFromFBX(fbxFile);
	}

	FILE* file = nullptr;
	fopen_s(&file, filepath.c_str(), "rb");
	assert(file);

	MeshAssetHeader header;
	MeshData data;

	assert(fread(&header, sizeof(MeshAssetHeader), 1, file));

	data.vertices.resize(header.vertexCount);
	assert(fread(data.vertices.data(), sizeof(Vertex), header.vertexCount, file));

	assert(fread(&data.boundingBox, sizeof(DirectX::BoundingBox), 1, file));

	data.skeleton.GetJoints().resize(header.boneCount);
	//Has to potential to read empty data, don't call assert()
	fread(data.skeleton.GetJoints().data(), sizeof(Joint), header.boneCount, file);

	fclose(file);
	
	existingMeshData.emplace(filename, data);
	auto& foundMeshData = existingMeshData.find(filename)->second;

	MeshDataProxy meshDataProxy;
	meshDataProxy.vertices = &foundMeshData.vertices;
	meshDataProxy.boundingBox = &foundMeshData.boundingBox;
	meshDataProxy.skeleton = &foundMeshData.skeleton;

	return meshDataProxy;
}

Animation AssetSystem::ReadVAnimAssetFromFile(const std::string filename)
{
	const std::string filepath = AssetBaseFolders::anim + filename;

	FILE* file = nullptr;
	fopen_s(&file, filepath.c_str(), "rb");
	assert(file);

	AnimationAssetHeader header;

	assert(fread(&header, sizeof(AnimationAssetHeader), 1, file));

	Animation anim = Animation(header.name);

	for (uint64_t i = 0; i < header.frameCount; i++)
	{
		int jointIndex = Joint::INVALID_JOINT_INDEX;
		assert(fread(&jointIndex, sizeof(int), 1, file));
		assert(jointIndex != -2);

		size_t animFrameCount = 0;
		assert(fread(&animFrameCount, sizeof(size_t), 1, file));

		std::vector<AnimFrame> animFrames;
		animFrames.resize(animFrameCount);
		assert(fread(animFrames.data(), sizeof(AnimFrame) * animFrameCount, 1, file));

		anim.AddFrame(jointIndex, animFrames);
	}

	fclose(file);

	return anim;
}

void AssetSystem::BuildAllGameplayMapFiles()
{
	for (auto const& dirEntry : std::filesystem::recursive_directory_iterator{ "WorldMaps" })
	{
		std::string file = dirEntry.path().filename().string();
		FileSystem::CreateGameplayWorldSave(file);
	}
}

void AssetSystem::WriteOutAllVertexColourData()
{
	FILE* file = nullptr;
	const std::string vertexColourFileFilename = VString::ReplaceFileExtesnion(World::worldFilename, vertexColourDataFileExtension);
	fopen_s(&file, vertexColourFileFilename.c_str(), "wb");
	assert(file);

	VertexColourHeader header;
	header.meshComponentCount = MeshComponent::system.GetNumComponents();
	fwrite(&header, sizeof(header), 1, file);

	for (auto& mesh : MeshComponent::system.GetComponents())
	{
		VertexColourData data;

		data.meshComponentUID = mesh->GetUID();
		fwrite(&data.meshComponentUID, sizeof(data.meshComponentUID), 1, file);

		data.numVertices = mesh->meshDataProxy.GetVertices().size();
		for (auto& vertex : mesh->meshDataProxy.GetVertices())
		{
			data.colours.push_back(vertex.colour);
		}

		fwrite(&data.numVertices, sizeof(data.numVertices), 1, file);
		fwrite(data.colours.data(), sizeof(DirectX::XMFLOAT4) * data.colours.size(), 1, file);
	}

	fclose(file);

	Log("Vertex colour data written to file [%s].", vertexColourFileFilename.c_str());
}

void AssetSystem::LoadVertexColourDataFromFile()
{
	FILE* file = nullptr;
	const std::string vertexColourFileFilename = VString::ReplaceFileExtesnion(World::worldFilename, vertexColourDataFileExtension);

	if (!std::filesystem::exists(vertexColourFileFilename))
	{
		Log("No vertex colour file data for world %s.", World::worldFilename.c_str());
		return;
	}

	fopen_s(&file, vertexColourFileFilename.c_str(), "rb");
	assert(file);

	VertexColourHeader header;
	fread(&header, sizeof(header), 1, file);

	for (int meshIndex = 0; meshIndex < header.meshComponentCount; meshIndex++)
	{
		VertexColourData vertexColourData;

		fread(&vertexColourData.meshComponentUID, sizeof(vertexColourData.meshComponentUID), 1, file);

		fread(&vertexColourData.numVertices, sizeof(vertexColourData.numVertices), 1, file);
		
		vertexColourData.colours.resize(vertexColourData.numVertices);
		fread(vertexColourData.colours.data(), sizeof(DirectX::XMFLOAT4) * vertexColourData.numVertices, 1, file);

		auto mesh = MeshComponent::system.GetComponentByUID(vertexColourData.meshComponentUID);
		assert(mesh);

		const size_t vertexCount = mesh->meshDataProxy.GetVertices().size();
		auto& verts = mesh->meshDataProxy.GetVertices();
		assert(vertexColourData.colours.size() == vertexCount);
		for (int vertexIndex = 0; vertexIndex < vertexCount; vertexIndex++)
		{
			verts[vertexIndex].colour = vertexColourData.colours[vertexIndex];
		}

		mesh->CreateNewVertexBuffer();
	}

	fclose(file);

	Log("Vertex colour data loaded from file [%s].", vertexColourFileFilename.c_str());
}
