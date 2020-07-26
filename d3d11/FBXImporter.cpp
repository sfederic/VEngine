#include "FBXImporter.h"
#include <vector>
#include "Debug.h"

//REF: https://github.com/peted70/hololens-fbx-viewer/tree/master/HolographicAppForOpenGLES1/include
//REF: https://peted.azurewebsites.net/hololens-fbx-loading-c/
//REF: https://help.autodesk.com/view/FBX/2020/ENU/

//NOTE: For the most part, not going to use FBX models with lights, cameras and extra nodes. One model should suffice.

FbxManager* manager;
FbxIOSettings* ioSetting;
FbxImporter* importer;

void FBXImporter::Init()
{
	manager = FbxManager::Create();
	ioSetting = FbxIOSettings::Create(manager, IOSROOT);
	importer = FbxImporter::Create(manager, "");
}

bool FBXImporter::Import(const char* filename, ModelData& data)
{
	if (!importer->Initialize(filename, -1, manager->GetIOSettings()))
	{
		DebugPrint("%s failed to load. Error Code: %d - %s\n", filename, 
			importer->GetStatus().GetCode(),
			importer->GetStatus().GetErrorString());
	}

	FbxScene* scene = FbxScene::Create(manager, "");
	importer->Import(scene);

	//Remember that the root node is essentially "empty"
	int nodeCount = scene->GetNodeCount();
	FbxNode* node = scene->GetNode(nodeCount - 1);

	FbxMesh* mesh = node->GetMesh();

	//Array setup
	int numVerts = mesh->GetControlPointsCount();
	int vectorSize = numVerts * mesh->GetPolygonSize(0);
	assert((vectorSize % 3) == 0);
	data.verts.reserve(vectorSize);

	//Geometry Elements
	FbxGeometryElementNormal* normals = mesh->GetElementNormal();
	FbxGeometryElementUV* uvs = mesh->GetElementUV();

	int polyIndexCounter = 0; //Used to index into normals and UVs on a per vertex basis
	int polyCount = mesh->GetPolygonCount();

	//Main import loop
	for (int i = 0; i < polyCount; i++)
	{
		int polySize = mesh->GetPolygonSize(i);
		assert((polySize % 3) == 0);

		for (int j = 0; j < polySize; j++)
		{
			int index = mesh->GetPolygonVertex(i, j);

			//TODO: indices are wrong. Stuck on ID3D11DeviceContext::Draw() without it
			data.indices.push_back(index);

			Vertex vert = {};
			
			//Position
			FbxVector4 pos = mesh->GetControlPointAt(index);
			vert.pos.x = (float)pos.mData[0];
			vert.pos.y = (float)pos.mData[1];
			vert.pos.z = (float)pos.mData[2];

			//UV
			int uvIndex = uvs->GetIndexArray().GetAt(polyIndexCounter);
			FbxVector2 uv = uvs->GetDirectArray().GetAt(uvIndex);
			vert.uv.x = (float)uv.mData[0];
			vert.uv.y = (float)uv.mData[1];

			//Normal
			FbxVector4 normal = normals->GetDirectArray().GetAt(polyIndexCounter);
			vert.normal.x = (float)normal.mData[0];
			vert.normal.y = (float)normal.mData[1];
			vert.normal.z = (float)normal.mData[2];

			data.verts.push_back(vert);

			polyIndexCounter++;
		}
	}

	mesh->Destroy();
	node->Destroy();
	scene->Destroy();

	return true;
}