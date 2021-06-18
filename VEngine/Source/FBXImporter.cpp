#include "FBXImporter.h"
#include <vector>
#include "Debug.h"
#include "Actor.h"
#include "RenderSystem.h"
#include "MathHelpers.h"
#include "Animationstructures.h"
#include <unordered_map>
#include <algorithm>
#include "Material.h"

//REF: https://github.com/peted70/hololens-fbx-viewer/tree/master/HolographicAppForOpenGLES1/include
//REF: https://peted.azurewebsites.net/hololens-fbx-loading-c/
//REF: https://help.autodesk.com/view/FBX/2020/ENU/

// Great GameDev.net reference on using FBX SDK and animation.
//REF: https://www.gamedev.net/tutorials/_/technical/graphics-programming-and-theory/how-to-work-with-fbx-sdk-r3582/

//NOTE: For the most part, not going to use FBX models with lights, cameras and extra nodes. One model should suffice. Import() code reflects that.
//NOTE: Models needto be triangluated, not working with control points (see asserts() below)

FbxManager* manager;
FbxIOSettings* ioSetting;
FbxImporter* importer;

FbxAnimEvaluator* animEvaluator;

ActorSystem* currentActorSystem;

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

	//Set importer fields.
	currentActorSystem = actorSystem;

	FbxScene* scene = FbxScene::Create(manager, "scene0");
	importer->Import(scene);

	//scene->GetGlobalSettings().SetAxisSystem(FbxAxisSystem::DirectX);

	animEvaluator = scene->GetAnimationEvaluator();

	FbxNode* rootNode = scene->GetRootNode();

	int childNodeCount = rootNode->GetChildCount();
	//for (int i = 0; i < childNodeCount; i++)
	{
		ProcessAllChildNodes(rootNode->GetChild(childNodeCount - 1));
	}

	scene->Destroy();

	currentActorSystem = nullptr;
	animEvaluator = nullptr;

	return true;
}

void FBXImporter::ProcessAllChildNodes(FbxNode* node)
{
	//Recursion code for dealing with nodes in the heirarchy. Come back to this after more work is done.
	/*int childNodeCount = node->GetChildCount();
	for (int i = 0; i < childNodeCount; i++)
	{
		ProcessAllChildNodes(node->GetChild(i));
	}*/

	FbxScene* scene = node->GetScene();

	/*FbxNodeAttribute* attrib = node->GetNodeAttribute();
	if (attrib->GetAttributeType() == FbxNodeAttribute::eSkeleton)
	{
		throw;
	}*/

	FbxInt nodeFlags = node->GetAllObjectFlags();
	if (nodeFlags & FbxPropertyFlags::eAnimated)
	{
		int numAnimStacks = scene->GetSrcObjectCount<FbxAnimStack>();
		for (int animStackIndex = 0; animStackIndex < numAnimStacks; animStackIndex++)
		{
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

								//This isn't too bad, but FBX file without animation still have eAnimated flags set
								//(for some reason) so placing this here is the quick fix. Better fix is doing this in the component.

								for (int keyIndex = 0; keyIndex < keyCount; keyIndex++)
								{
									//Keys are the keyframes into the animation
									double keyTime = animCurve->KeyGet(keyIndex).GetTime().GetSecondDouble();
									FbxTime time;
									time.SetSecondDouble(keyTime);

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

								}
							}
						}
					}
				}
			}
		}
	}

	//std::unordered_map<int, BoneWeights> boneWeightsMap;

	FbxMesh* mesh = node->GetMesh();
	if (mesh)
	{
		//OLD WEIGHT AND BONE INDICES CODE
		/*const int deformerCount = mesh->GetDeformerCount(FbxDeformer::eSkin);
		for (int deformerIndex = 0; deformerIndex < deformerCount; deformerIndex++)
		{
			FbxSkin* skin = (FbxSkin*)mesh->GetDeformer(deformerIndex, FbxDeformer::eSkin);
			
			const int clusterCount = skin->GetClusterCount();
			for (int clusterIndex = 0; clusterIndex < clusterCount; clusterIndex++)
			{
				FbxCluster* cluster = skin->GetCluster(clusterIndex);
				const int vertexIndexCount = cluster->GetControlPointIndicesCount();
				for (int i = 0; i < vertexIndexCount; i++)
				{
					int index = cluster->GetControlPointIndices()[i];

					if (index >= vertexIndexCount)
					{
						continue;
					}

					double weight = cluster->GetControlPointWeights()[i];

					if (weight == 0.0 || weight >= 1.0)
					{
						continue;
					}

					if (boneWeightsMap[index].boneIndex.size() < 4)
					{
						boneWeightsMap[index].boneIndex.push_back(currentBoneIndex);
					}

					if (boneWeightsMap[index].weights.size() < 3)
					{
						boneWeightsMap[index].weights.push_back(weight);
					}
				}
			}
		}*/

		//Material 
		int materialCount = node->GetMaterialCount();
		for (int materialIndex = 0; materialIndex < materialCount; materialIndex++)
		{
			FbxSurfacePhong* material = (FbxSurfacePhong*)node->GetMaterial(materialIndex);
			FbxClassId surfaceID = material->GetClassId();
			FbxDouble3 ambient = material->Ambient.Get();

			Material mat = {};
			mat.colour.x = ambient.mData[0];
			mat.colour.y = ambient.mData[1];
			mat.colour.z = ambient.mData[2];
			mat.colour.w = 1.0f;
		}

		//Array setup
		int numVerts = mesh->GetControlPointsCount();
		int vectorSize = numVerts * mesh->GetPolygonSize(0);
		assert((vectorSize % 3) == 0 && "FBX model isn't triangulated"); //This is a check to make sure the mesh is triangulated (in blender, Ctrl+T)

		//Geometry Elements
		FbxGeometryElementNormal* normals = mesh->GetElementNormal();
		FbxGeometryElementUV* uvs = mesh->GetElementUV();

		int polyIndexCounter = 0; //Used to index into normals and UVs on a per vertex basis
		int polyCount = mesh->GetPolygonCount();

		//Main import loop
		for (int i = 0; i < polyCount; i++)
		{
			int polySize = mesh->GetPolygonSize(i);
			assert((polySize % 3) == 0 && "FBX model isn't triangulated");

			for (int j = 0; j < polySize; j++)
			{
				int index = mesh->GetPolygonVertex(i, j);

				//TODO: indices are wrong without the vertex posisitons being compressed down
				currentActorSystem->modelData.indices.push_back(index);

				Vertex vert = {};

				//Position
				FbxVector4 pos = mesh->GetControlPointAt(index);
				vert.pos.x = (float)pos.mData[0];
				vert.pos.y = (float)pos.mData[1];
				vert.pos.z = (float)pos.mData[2];

				//UV
				if (uvs)
				{
					int uvIndex = uvs->GetIndexArray().GetAt(polyIndexCounter);
					FbxVector2 uv = uvs->GetDirectArray().GetAt(uvIndex);
					vert.uv.x = (float)uv.mData[0];
					vert.uv.y = (float)uv.mData[1];
				}

				//Normal
				if (normals)
				{
					FbxVector4 normal = normals->GetDirectArray().GetAt(polyIndexCounter);
					vert.normal.x = (float)normal.mData[0];
					vert.normal.y = (float)normal.mData[1];
					vert.normal.z = (float)normal.mData[2];
				}

				//Bone Weights
				/*if (boneWeightsMap.find(index) != boneWeightsMap.end())
				{
					BoneWeights* boneData = &boneWeightsMap.find(index)->second;
					if (boneData)
					{
						//There must be a way to merge the above cluster FBX code and the vertices.
						for (int i = 0; i < boneData->weights.size(); i++)
						{
							vert.weights[i] = boneData->weights[i];
						}

						for (int i = 0; i < boneData->weights.size(); i++)
						{
							vert.boneIndices[i] = boneData->boneIndex[i];
						}
					}
				}*/

				currentActorSystem->modelData.verts.push_back(vert);

				polyIndexCounter++;
			}
		}
	}
}