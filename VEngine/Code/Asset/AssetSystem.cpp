#include "vpch.h"
#include "AssetSystem.h"
#include <filesystem>
#include <qfiledialog.h>
#include "FBXLoader.h"
#include "MeshAssetHeader.h"
#include "AssetFileExtensions.h"
#include "AnimationAssetHeader.h"
#include "Core/Profile.h"
#include "Core/Log.h"
#include "Core/FileSystem.h"
#include "Core/WorldEditor.h"
#include "Core/VString.h"
#include "Components/MeshComponent.h"
#include "Asset/AssetBaseFolders.h"
#include "Asset/VertexColourData.h"
#include "Asset/VertexColourHeader.h"
#include "Render/MeshData.h"
#include "Render/Vertex.h"
#include "Render/Renderer.h"

struct FBXFileInfo
{
	std::string filepath;
	std::string filename;
};

std::unordered_map<std::string, MeshData> existingMeshData;

static const std::string vertexColourDataFileExtension = ".vertexcolourdata";

void AssetSystem::ResetMeshData()
{
	existingMeshData.clear();
}

void AssetSystem::BuildAllVMeshDataFromFBXImport()
{
	uint64_t numberOfMeshFilesBuilt = 0;

	auto startTime = Profile::QuickStart();

	std::vector<FBXFileInfo> fbxFileInfos;

	//Import all FBX files
	for (const auto& entry : std::filesystem::recursive_directory_iterator("FBXFiles/"))
	{
		if (entry.is_directory())
		{
			continue;
		}

		FBXFileInfo fbxFileInfo;
		fbxFileInfo.filename = entry.path().filename().string();
		std::string path = std::filesystem::absolute(entry.path()).string();
		std::replace(path.begin(), path.end(), '\\', '/');
		fbxFileInfo.filepath = path;
		fbxFileInfos.emplace_back(fbxFileInfo);
	}

	for (const auto& fileInfo : fbxFileInfos)
	{
		AssetSystem::BuildSingleVMeshFromFBX(fileInfo.filepath, fileInfo.filename);
		Renderer::Get().SetRendererToCaptureMeshIcon(fileInfo.filename);
		numberOfMeshFilesBuilt++;
	}

	double elapsedTime = Profile::QuickEnd(startTime);

	Log("Mesh asset build complete.\n\tNum meshes built: %d\n\tTime taken: %f",
		numberOfMeshFilesBuilt, elapsedTime);
}

void AssetSystem::CreateVMeshFromInWorldMesh()
{
	auto actor = WorldEditor::GetPickedActor();
	if (actor)
	{
		auto meshes = actor->GetComponents<MeshComponent>();
		for (auto mesh : meshes)
		{
			MeshData meshData = {};
			XMStoreFloat3(&meshData.boundingBox.Center, mesh->GetBoundsCenter());
			XMStoreFloat3(&meshData.boundingBox.Extents, mesh->GetBoundsExtents());
			meshData.vertices = mesh->GetAllVertices();

			MeshAssetHeader header = {};
			header.sourceMeshFormat = SourceMeshFormat::FBX;
			header.vertexCount = meshData.vertices.size();
			header.boneCount = meshData.skeleton.GetNumJoints();

			QFileDialog dialog;
			dialog.setFileMode(QFileDialog::AnyFile);

			const QString meshFilename = QString::fromStdString(AssetBaseFolders::mesh + mesh->GetMeshFilename());
			const QString meshFile = dialog.getSaveFileName(nullptr,
				"Create new vmesh",
				meshFilename,
				QString::fromStdString(AssetFileExtensions::mesh),
				nullptr,
				QFileDialog::Option::DontUseNativeDialog);

			FILE* file = nullptr;
			fopen_s(&file, meshFile.toStdString().c_str(), "wb");
			assert(file);

			fwrite(&header, sizeof(MeshAssetHeader), 1, file);
			fwrite(meshData.vertices.data(), sizeof(Vertex), meshData.vertices.size(), file);
			fwrite(&meshData.boundingBox, sizeof(DirectX::BoundingBox), 1, file);

			auto& joints = meshData.skeleton.GetJoints();
			fwrite(joints.data(), sizeof(Joint), joints.size(), file);

			fclose(file);
		}
	}
}

void AssetSystem::BuildAllAnimationFilesFromFBXImport()
{
	uint64_t numberOfAnimationFilesBuilt = 0;

	auto startTime = Profile::QuickStart();

	std::vector<FBXFileInfo> fileInfos;

	for (const auto& entry : std::filesystem::recursive_directory_iterator("AnimationFBXFiles/"))
	{
		if (entry.is_directory())
		{
			continue;
		}

		FBXFileInfo fileInfo;
		fileInfo.filename = entry.path().filename().string();
		std::string path = std::filesystem::absolute(entry.path()).string();
		std::replace(path.begin(), path.end(), '\\', '/');
		fileInfo.filepath = path;
		fileInfos.emplace_back(fileInfo);
	}

	for (const auto& fileInfo : fileInfos)
	{
		AssetSystem::BuildSingleVAnimFromFBX(fileInfo.filepath, fileInfo.filename);
		numberOfAnimationFilesBuilt++;
	}

	double elapsedTime = Profile::QuickEnd(startTime);

	Log("Animation asset build complete.\nNum animations built: %d\n\tTime taken: %f",
		numberOfAnimationFilesBuilt, elapsedTime);
}

void AssetSystem::BuildSingleVMeshFromFBX(const std::string fbxFilePath, const std::string fbxFilename)
{
	const std::string baseFBXPath = VString::GetSubStringAtFoundOffset(fbxFilePath, AssetBaseFolders::fbxFiles);
	const std::string vMeshPath = VString::ReplaceFileExtesnion(baseFBXPath, ".vmesh");
	const std::filesystem::path filepath = AssetBaseFolders::mesh + vMeshPath;
	const auto meshFilePath = std::filesystem::absolute(filepath);

	MeshData meshData;
	FBXLoader::ImportAsMesh(fbxFilePath, meshData);

	MeshAssetHeader header;
	header.sourceMeshFormat = SourceMeshFormat::FBX;
	header.vertexCount = meshData.vertices.size();
	header.boneCount = meshData.skeleton.GetNumJoints();

	//Note: Make sure there's a matching folder for meshes from where the fbx file came from. 
	FILE* file = nullptr;
	fopen_s(&file, meshFilePath.string().c_str(), "wb");
	assert(file);

	assert(fwrite(&header, sizeof(MeshAssetHeader), 1, file));
	assert(fwrite(meshData.vertices.data(), sizeof(Vertex), meshData.vertices.size(), file));
	assert(fwrite(&meshData.boundingBox, sizeof(DirectX::BoundingBox), 1, file));

	auto& joints = meshData.skeleton.GetJoints();
	fwrite(joints.data(), sizeof(Joint), joints.size(), file);

	fclose(file);
}

void AssetSystem::BuildSingleVAnimFromFBX(const std::string fbxAnimFilePath, const std::string fbxAnimFilename)
{
	auto animations = FBXLoader::ImportAsAnimation(fbxAnimFilePath, fbxAnimFilename);

	const std::string baseFBXAnimPath = VString::GetSubStringAtFoundOffset(fbxAnimFilePath, AssetBaseFolders::animationFBXFiles);
	const std::string vAnimPath = VString::ReplaceFileExtesnion(baseFBXAnimPath, ".vanim");
	const std::filesystem::path filepath = AssetBaseFolders::anim + vAnimPath;
	const auto animFilePath = std::filesystem::absolute(filepath);

	//Note: Make sure there's a matching folder for animations from where the fbx file came from. 
	FILE* file = nullptr;
	fopen_s(&file, animFilePath.string().c_str(), "wb");
	assert(file);

	{
		AnimationAssetHeader header;
		header.animationCount = animations.size();
		assert(fwrite(&header, sizeof(AnimationAssetHeader), 1, file));
	}

	for (auto& [animName, animation] : animations)
	{
		AnimationFrameHeader animFrameHeader = {};
		strcpy_s(animFrameHeader.name, sizeof(char) * Animation::ANIM_NAME_MAX, animName.c_str());
		animFrameHeader.frameCount = animation.GetFrames().size();
		assert(fwrite(&animFrameHeader, sizeof(AnimationFrameHeader), 1, file));

		for (auto& [jointIndex, animFrame] : animation.GetFrames())
		{
			assert(fwrite(&jointIndex, sizeof(int), 1, file));

			const size_t animFrameCount = animFrame.size();
			assert(fwrite(&animFrameCount, sizeof(size_t), 1, file));
			assert(fwrite(animFrame.data(), sizeof(AnimFrame), animFrameCount, file));
		}
	}

	fclose(file);
}

MeshDataProxy AssetSystem::ReadVMeshAssetFromFile(const std::string filename)
{
	std::string filepath = AssetBaseFolders::mesh + filename;

	//Create VMesh if it doesn't exist yet.
	if (!std::filesystem::exists(filepath))
	{
		const std::string fbxFile = VString::ReplaceFileExtesnion(filename, ".fbx");
		const std::string fbxFilePath = std::filesystem::current_path().string() +
			"\\" + AssetBaseFolders::fbxFiles + fbxFile;
		BuildSingleVMeshFromFBX(fbxFile, fbxFilePath);
	}

	FILE* file = nullptr;
	fopen_s(&file, filepath.c_str(), "rb");
	assert(file);

	MeshAssetHeader header;
	MeshData data;

	fread(&header, sizeof(MeshAssetHeader), 1, file);

	data.vertices.resize(header.vertexCount);
	fread(data.vertices.data(), sizeof(Vertex), header.vertexCount, file);

	fread(&data.boundingBox, sizeof(DirectX::BoundingBox), 1, file);

	data.skeleton.GetJoints().resize(header.boneCount);
	//Has to potential to read empty data, don't call assert()
	fread(data.skeleton.GetJoints().data(), sizeof(Joint), header.boneCount, file);

	fclose(file);

	existingMeshData.emplace(filename, data);
	auto& foundMeshData = existingMeshData.find(filename)->second;

	MeshDataProxy meshDataProxy;
	meshDataProxy.vertices = foundMeshData.vertices;
	meshDataProxy.boundingBox = &foundMeshData.boundingBox;
	meshDataProxy.skeleton = &foundMeshData.skeleton;

	return meshDataProxy;
}

std::vector<Animation> AssetSystem::ReadVAnimAssetFromFile(const std::string filename)
{
	const std::string filepath = AssetBaseFolders::anim + filename;

	FILE* file = nullptr;
	fopen_s(&file, filepath.c_str(), "rb");
	assert(file);

	AnimationAssetHeader header = {};
	assert(fread(&header, sizeof(AnimationAssetHeader), 1, file));

	std::vector<Animation> animations;

	for (int i = 0; i < header.animationCount; i++)
	{
		AnimationFrameHeader animFrameHeader = {};
		assert(fread(&animFrameHeader, sizeof(AnimationFrameHeader), 1, file));

		Animation anim = Animation(animFrameHeader.name);

		for (int animFrameIndex = 0; animFrameIndex < animFrameHeader.frameCount; animFrameIndex++)
		{
			int jointIndex = Joint::INVALID_JOINT_INDEX;
			fread(&jointIndex, sizeof(int), 1, file);
			assert(jointIndex != Joint::INVALID_JOINT_INDEX);

			size_t animFrameCount = 0;
			fread(&animFrameCount, sizeof(size_t), 1, file);

			std::vector<AnimFrame> animFrames;
			animFrames.resize(animFrameCount);
			fread(animFrames.data(), sizeof(AnimFrame) * animFrameCount, 1, file);

			anim.AddFrame(jointIndex, animFrames);
		}

		animations.emplace_back(anim);
	}

	fclose(file);

	return animations;
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
	const std::string vertexColourFileFilename = AssetBaseFolders::vertexColourData +
		VString::ReplaceFileExtesnion(World::Get().worldFilename, vertexColourDataFileExtension);
	FILE* file = nullptr;
	fopen_s(&file, vertexColourFileFilename.c_str(), "wb");
	assert(file);

	VertexColourHeader header;
	header.meshComponentCount = MeshComponent::system.GetNumComponents();
	fwrite(&header, sizeof(header), 1, file);

	std::set<UID> uniqueMeshUIDs;

	for (auto& mesh : MeshComponent::system.GetComponents())
	{
		VertexColourData data;

		assert(uniqueMeshUIDs.find(mesh->GetUID()) == uniqueMeshUIDs.end());
		uniqueMeshUIDs.emplace(mesh->GetUID());

		data.meshComponentUID = mesh->GetUID();
		fwrite(&data.meshComponentUID, sizeof(data.meshComponentUID), 1, file);

		data.numVertices = mesh->meshDataProxy.GetVertices().size();
		for (auto& vertex : mesh->meshDataProxy.GetVertices())
		{
			data.colours.emplace_back(vertex.colour);
		}

		fwrite(&data.numVertices, sizeof(data.numVertices), 1, file);
		fwrite(data.colours.data(), sizeof(DirectX::XMFLOAT4) * data.colours.size(), 1, file);
	}

	fclose(file);

	Log("Vertex colour data written to file [%s].", vertexColourFileFilename.c_str());
}

void AssetSystem::LoadVertexColourDataFromFile()
{
	const std::string vertexColourFileFilename = AssetBaseFolders::vertexColourData +
		VString::ReplaceFileExtesnion(World::Get().worldFilename, vertexColourDataFileExtension);

	LoadVertexColourDataFromFilename(vertexColourFileFilename);
}

void AssetSystem::LoadVertexColourDataFromFilename(const std::string filename)
{
	FILE* file = nullptr;
	if (!std::filesystem::exists(filename))
	{
		Log("No vertex colour file data for world %s.", World::Get().worldFilename.c_str());
		return;
	}

	fopen_s(&file, filename.c_str(), "rb");
	assert(file);

	VertexColourHeader header;
	fread(&header, sizeof(header), 1, file);

	std::set<UID> uniqueMeshUIDs;

	for (int meshCount = 0; meshCount < header.meshComponentCount; meshCount++)
	{
		VertexColourData vertexColourData;

		fread(&vertexColourData.meshComponentUID, sizeof(vertexColourData.meshComponentUID), 1, file);
		assert(uniqueMeshUIDs.find(vertexColourData.meshComponentUID) == uniqueMeshUIDs.end());
		uniqueMeshUIDs.emplace(vertexColourData.meshComponentUID);

		fread(&vertexColourData.numVertices, sizeof(vertexColourData.numVertices), 1, file);

		vertexColourData.colours.resize(vertexColourData.numVertices);
		fread(vertexColourData.colours.data(), sizeof(DirectX::XMFLOAT4) * vertexColourData.colours.size(), 1, file);

		auto mesh = MeshComponent::system.GetComponentByUID(vertexColourData.meshComponentUID);
		if (mesh == nullptr)
		{
			continue;
		}

		const size_t vertexCount = mesh->meshDataProxy.GetVertices().size();
		if (vertexColourData.colours.size() != vertexCount)
		{
			Log("Mismatch of vertex colour data size and vertex count for mesh [%u] on Actor [%s].",
				mesh->GetUID(), mesh->GetOwner()->GetName().c_str());
			continue;
		}

		for (int vertexIndex = 0; vertexIndex < vertexCount; vertexIndex++)
		{
			mesh->meshDataProxy.vertices.at(vertexIndex).colour = vertexColourData.colours[vertexIndex];
		}

		mesh->CreateNewVertexBuffer();
	}

	fclose(file);

	Log("Vertex colour data loaded from file [%s].", filename.c_str());
}
