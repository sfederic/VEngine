export module Asset.AssetSystem;

import <vector>;
import <unordered_map>;
import <string>;
import Render.MeshData;
import Render.MeshDataProxy;
import Animation.Animation;

export class AssetSystem
{
public:
	static AssetSystem Get()
	{
		static AssetSystem instance;
		return instance;
	}

	void ResetMeshData();

	void BuildAllVMeshDataFromFBXImport();
	void CreateVMeshFromInWorldMesh();
	void BuildAllAnimationFilesFromFBXImport();

	void BuildSingleVMeshFromFBX(const std::string fbxFilePath, const std::string fbxFilename);
	void BuildSingleVAnimFromFBX(const std::string fbxAnimFilePath, const std::string fbxAnimFilename);

	MeshDataProxy ReadVMeshAssetFromFile(const std::string filename);
	std::vector<Animation> ReadVAnimAssetFromFile(const std::string filename);

	void BuildAllGameplayMapFiles();

	void WriteOutAllVertexColourData();
	void LoadVertexColourDataFromFile();
	void LoadVertexColourDataFromFilename(const std::string filename);

private:
	struct FBXFileInfo
	{
		std::string filepath;
		std::string filename;
	};

	std::unordered_map<std::string, MeshData> existingMeshData;

	inline static const std::string vertexColourDataFileExtension = ".vertexcolourdata";
};
