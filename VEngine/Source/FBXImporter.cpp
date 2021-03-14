#include "FBXImporter.h"
#include <vector>
#include "Debug.h"
#include "Actor.h"
#include "RenderSystem.h"

//REF: https://github.com/peted70/hololens-fbx-viewer/tree/master/HolographicAppForOpenGLES1/include
//REF: https://peted.azurewebsites.net/hololens-fbx-loading-c/
//REF: https://help.autodesk.com/view/FBX/2020/ENU/

//NOTE: For the most part, not going to use FBX models with lights, cameras and extra nodes. One model should suffice. Import() code reflects that.
//NOTE: Models needto be triangluated, not working with control points (see asserts() below)

FbxManager* manager;
FbxIOSettings* ioSetting;
FbxImporter* importer;

void FBXImporter::Init()
{
	manager = FbxManager::Create();
	ioSetting = FbxIOSettings::Create(manager, IOSROOT);
	importer = FbxImporter::Create(manager, "");
}

struct AnimData
{
	FbxVector4 rot;
	double time;
};

bool FBXImporter::Import(const char* filename, ModelData& data)
{
	if (!importer->Initialize(filename, -1, manager->GetIOSettings()))
	{
		DebugPrint("%s failed to load. Error Code: %d - %s\n", filename, 
			importer->GetStatus().GetCode(),
			importer->GetStatus().GetErrorString());
	}

	FbxScene* scene = FbxScene::Create(manager, "scene0");
	importer->Import(scene);

	//Remember that the root node is essentially "empty"
	int nodeCount = scene->GetNodeCount();
	FbxNode* node = scene->GetNode(nodeCount - 1);

	FbxMesh* mesh = node->GetMesh();

	//TODO: ANIMATION
	//Essentially the fix for skeletal animation is to iterate through all nodes in the scene,
	//then take their times from the animation keys and apply it to a vector or matrix which can be 
	//part of a struct. 
	FbxInt nodeFlags = node->GetAllObjectFlags();
	if (nodeFlags & FbxPropertyFlags::eAnimated)
	{
		FbxAnimEvaluator* animEvaluator = scene->GetAnimationEvaluator();

		int numAnimStacks = scene->GetSrcObjectCount<FbxAnimStack>();
		for (int animStackIndex = 0; animStackIndex < numAnimStacks; animStackIndex++)
		{
			FbxAnimStack* animStack = scene->GetSrcObject<FbxAnimStack>(animStackIndex);
			//if (animStack)
			{
				int numAnimLayers = animStack->GetMemberCount<FbxAnimLayer>();
				for (int animLayerIndex = 0; animLayerIndex < numAnimLayers; animLayerIndex++)
				{
					FbxAnimLayer* animLayer = animStack->GetMember<FbxAnimLayer>(animLayerIndex);
					//if (animLayer)
					{
						FbxAnimCurveNode* curveNode = node->LclRotation.GetCurveNode(animLayer);
						int numCurveNodes = curveNode->GetCurveCount(0);

						for(int curveIndex = 0; curveIndex < numCurveNodes; curveIndex++)
						{
							FbxAnimCurve* animCurve = curveNode->GetCurve(curveIndex);
							int keyCount = animCurve->KeyGetCount();

							std::vector<AnimData> data;

							for (int keyIndex = 0; keyIndex < keyCount; keyIndex++)
							{
								//Keys are the keyframes into the animation
								double keyTime = animCurve->KeyGet(keyIndex).GetTime().GetSecondDouble();
								FbxTime time;
								time.SetSecondDouble(keyTime);

								FbxVector4 rot = animEvaluator->GetNodeLocalRotation(node, time);

								AnimData animdata = {};
								animdata.time = keyTime;
								animdata.rot = rot;
								data.push_back(animdata);
							}
						}
					}
				}
			}
		}
	}

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

			//TODO: indices are wrong without the vertex posisitons being compressed down
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