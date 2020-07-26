#include "FBXImporter.h"
#include <vector>
#include "Debug.h"

//REF: https://github.com/peted70/hololens-fbx-viewer/tree/master/HolographicAppForOpenGLES1/include
//REF: https://peted.azurewebsites.net/hololens-fbx-loading-c/
//REF: https://help.autodesk.com/view/FBX/2020/ENU/

//NOTE: For the most part, not going to use FBX models with lights, cameras and extra nodes. One model should suffice.

bool FBXImporter::Import(const char* filename, ModelData& data)
{
	FbxManager* manager = FbxManager::Create();
	FbxIOSettings* ioSetting = FbxIOSettings::Create(manager, IOSROOT);

	FbxImporter* importer = FbxImporter::Create(manager, "");
	if (!importer->Initialize(filename, -1, manager->GetIOSettings()))
	{
		DebugPrint("%s failed to load. Error Code: %d - %s\n", filename, 
			importer->GetStatus().GetCode(),
			importer->GetStatus().GetErrorString());
	}


	int lFileMajor, lFileMinor, lFileRevision;
	importer->GetFileVersion(lFileMajor, lFileMinor, lFileRevision);

	FbxScene* scene = FbxScene::Create(manager, "testScene");
	importer->Import(scene);
	importer->Destroy();

	//Remember that the root node is essentially "empty"
	int nodeCount = scene->GetNodeCount();
	FbxNode* node = scene->GetNode(nodeCount - 1);

	FbxMesh* mesh = node->GetMesh();

	int verts = mesh->GetControlPointsCount();

	data.verts.reserve(verts);



	int polyCount = mesh->GetPolygonCount();

	int vertNum = 0;

	for (int i = 0; i < polyCount; i++)
	{
		int polySize = mesh->GetPolygonSize(i);

		for (int j = 0; j < polySize; j++)
		{
			int index = mesh->GetPolygonVertex(i, j);
			data.indices.push_back(index);

			VertexUVNormal vert = {};

			//UV
			FbxGeometryElementUV* uvs = mesh->GetElementUV();
			FbxVector2 uv = {};
			if (uvs->GetReferenceMode() == FbxGeometryElement::eByControlPoint)
			{
				if (uvs->GetReferenceMode() == FbxGeometryElement::eDirect)
				{
					uv = uvs->GetDirectArray().GetAt(index);
				}
				else
				{
					int uvIndex = uvs->GetIndexArray().GetAt(index);
					uvs->GetDirectArray().GetAt(uvIndex);
				}
			}
			else if (uvs->GetReferenceMode() == FbxGeometryElement::eByPolygonVertex)
			{
				if (uvs->GetReferenceMode() == FbxGeometryElement::eDirect)
				{
					uv = uvs->GetDirectArray().GetAt(vertNum);
				}
				else
				{
					int uvIndex = uvs->GetIndexArray().GetAt(vertNum);
					uvs->GetDirectArray().GetAt(uvIndex);
				}
			}

			vert.uv.x = uv.mData[0];
			vert.uv.y = uv.mData[1];

			//Normal
			FbxGeometryElementNormal* normals = mesh->GetElementNormal();
			FbxVector4 normal = {};

			if (normals->GetReferenceMode() == FbxGeometryElement::eByControlPoint)
			{
				if (normals->GetReferenceMode() == FbxGeometryElement::eDirect)
				{
					normal = normals->GetDirectArray().GetAt(index);
				}
				else
				{
					int normalIndex = normals->GetIndexArray().GetAt(index);
					normal = normals->GetDirectArray().GetAt(normalIndex);
				}
			}
			else if (normals->GetReferenceMode() == FbxGeometryElement::eByPolygonVertex)
			{
				if (normals->GetReferenceMode() == FbxGeometryElement::eDirect)
				{
					normal = normals->GetDirectArray().GetAt(vertNum);
				}
				else
				{
					int normalIndex = normals->GetIndexArray().GetAt(vertNum);
					normal = normals->GetDirectArray().GetAt(normalIndex);
				}
			}

			vert.normal.x = normal.mData[0];
			vert.normal.y = normal.mData[1];
			vert.normal.z = normal.mData[2];

			vertNum++;

			data.uvsNormals.push_back(vert);
		}
	}

	for (int i = 0; i < verts; i++)
	{
		XMFLOAT3 vert = {};

		//Position
		FbxVector4 pos = mesh->GetControlPointAt(i);
		vert.x = pos.mData[0];
		vert.y = pos.mData[1];
		vert.z = pos.mData[2];

		//UV
		/*FbxGeometryElementUV* uvs = mesh->GetElementUV();
		FbxVector2 uv = uvs->GetDirectArray().GetAt(i);
		vert.uv.x = uv.mData[0];
		vert.uv.y = uv.mData[1];

		//Normal
		FbxGeometryElementNormal* normals = mesh->GetElementNormal(); 
		FbxVector4 normal = normals->GetDirectArray().GetAt(i);
		vert.normal.x = normal.mData[0];
		vert.normal.y = normal.mData[1];
		vert.normal.z = normal.mData[2];*/

		data.verts.push_back(vert);
	}

	return true;
}