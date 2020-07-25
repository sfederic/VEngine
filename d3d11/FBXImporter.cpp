#include "FBXImporter.h"
#include <vector>
#include "Debug.h"

//REF: https://github.com/peted70/hololens-fbx-viewer/tree/master/HolographicAppForOpenGLES1/include
//REF: https://peted.azurewebsites.net/hololens-fbx-loading-c/

//TODO: all testing code. fix up. HOw do you get indices?
void FBXImporter::Import()
{
	FbxManager* manager = FbxManager::Create();
	FbxIOSettings* ioSetting = FbxIOSettings::Create(manager, IOSROOT);

	FbxImporter* importer = FbxImporter::Create(manager, "");
	if (!importer->Initialize("plane.fbx", -1, manager->GetIOSettings()))
	{
		DebugPrint("model not found blah bah");
	}

	FbxScene* scene = FbxScene::Create(manager, "myScene");
	importer->Import(scene);
	importer->Destroy();

	FbxNode* node = nullptr;

	int nodeCount = scene->GetNodeCount();
	node = scene->GetNode(nodeCount - 1);

	FbxMesh* mesh = node->GetMesh();

	int verts = mesh->GetControlPointsCount();

	std::vector<fbxsdk::FbxVector4> vec;

	for (int i = 0; i < verts; i++)
	{
		fbxsdk::FbxVector4 vec4 = mesh->GetControlPointAt(i);
		vec.push_back(vec4);
	}
}