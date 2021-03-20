#include "FBXImporter.h"
#include <vector>
#include "Debug.h"
#include "Actor.h"
#include "RenderSystem.h"
#include "Animationstructures.h"

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

bool FBXImporter::Import(const char* filename, ModelData& data, ActorSystem* actorSystem)
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
	//TODO: going to have to come back here later for Skeletal Animation, as every bone is a node.
	//Also, go through and do the same recursion for every forloop.
	FbxNode* rootNode = scene->GetRootNode();

	const int nodeCount = rootNode->GetChildCount();
	for (int nodeIndex = 0; nodeIndex < nodeCount; nodeIndex++)
	{
		//TODO: this is shit, you need a recursive funcion here to deal with potential node children off of this node
		FbxNode* node = rootNode->GetChild(nodeIndex);
		if (node == nullptr) { continue; }

		FbxInt nodeFlags = node->GetAllObjectFlags();
		if (nodeFlags & FbxPropertyFlags::eAnimated)
		{
			FbxAnimEvaluator* animEvaluator = scene->GetAnimationEvaluator();

			int numAnimStacks = scene->GetSrcObjectCount<FbxAnimStack>();
			for (int animStackIndex = 0; animStackIndex < numAnimStacks; animStackIndex++)
			{
				//TODO: This isn't too bad, but FBX file without animation still have eAnimated flags set,
				//for some reason, so plaing this here is the quick fix. Better fix is doing this in the component.
				actorSystem->bAnimated = true;

				FbxAnimStack* animStack = scene->GetSrcObject<FbxAnimStack>(animStackIndex);
				if (animStack)
				{
					int numAnimLayers = animStack->GetMemberCount<FbxAnimLayer>();
					for (int animLayerIndex = 0; animLayerIndex < numAnimLayers; animLayerIndex++)
					{
						FbxAnimLayer* animLayer = animStack->GetMember<FbxAnimLayer>(animLayerIndex);
						if (animLayer)
						{
							//Feels like just getting one curve isn't the right answer here.
							FbxAnimCurveNode* curveNode = node->LclRotation.GetCurveNode(animLayer);
							if (curveNode)
							{
								int numCurveNodes = curveNode->GetCurveCount(0);

								for (int curveIndex = 0; curveIndex < numCurveNodes; curveIndex++)
								{
									FbxAnimCurve* animCurve = curveNode->GetCurve(curveIndex);
									int keyCount = animCurve->KeyGetCount();
									
									for (int keyIndex = 0; keyIndex < keyCount; keyIndex++)
									{
										//Keys are the keyframes into the animation
										double keyTime = animCurve->KeyGet(keyIndex).GetTime().GetSecondDouble();
										FbxTime time;
										time.SetSecondDouble(keyTime);

										//TODO: have to comeback here and clean up Quat conversions, uniform scaling,
										//decide whether to even use transform (in the sense that all animations just play 
										//in-place.)
										FbxVector4 rot = animEvaluator->GetNodeLocalRotation(node, time);
										FbxVector4 scale = animEvaluator->GetNodeLocalScaling(node, time);
										FbxVector4 pos = animEvaluator->GetNodeLocalTransform(node, time);

										AnimFrame animFrame = {};
										animFrame.time = keyTime;

										XMVECTOR euler = XMVectorZero();
										//Angles are measured clockwise when looking along the rotation axis toward the 
										//origin. This is a left-handed coordinate system. 
										//To use right-handed coordinates, negate all three angles.
										euler.m128_f32[0] = -XMConvertToRadians(rot[0]);
										euler.m128_f32[1] = -XMConvertToRadians(rot[1]);
										euler.m128_f32[2] = -XMConvertToRadians(rot[2]);
										XMVECTOR quat = XMQuaternionRotationRollPitchYawFromVector(euler);
										animFrame.rot.x = quat.m128_f32[0];
										animFrame.rot.y = quat.m128_f32[1];
										animFrame.rot.z = quat.m128_f32[2];
										animFrame.rot.w = quat.m128_f32[3];

										animFrame.scale.x = 1.f;
										animFrame.scale.y = 1.f;
										animFrame.scale.z = 1.f;

										//animFrame.pos.x = pos[0];
										//animFrame.pos.y = pos[1];
										//animFrame.pos.z = pos[2];

										actorSystem->animData.frames.push_back(animFrame);
									}
								}
							}
						}
					}
				}
			}
		}

		FbxMesh* mesh = node->GetMesh();
		if (mesh)
		{
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
		}
	}

	//Cleanup
	scene->Destroy();

	return true;
}