#include "FBXLoader.h"
#include <cassert>
#include <filesystem>
#include "Animation/AnimationStructures.h"
#include "VMath.h"

FBXLoader fbxLoader;

void FBXLoader::Init()
{
	manager = FbxManager::Create();
	ioSetting = FbxIOSettings::Create(manager, IOSROOT);
	importer = FbxImporter::Create(manager, "");
}

bool FBXLoader::Import(std::string filename, MeshDataProxy* meshData)
{
	std::string filepath = "Meshes/" + filename;
	
	if (filename.empty() || !std::filesystem::exists(filepath))
	{
		//set default model
		filename = "cube.fbx";
		filepath = "Meshes/cube.fbx";
	}

	//Find if mesh data already exists, push it to meshcomponent data
	auto existingMeshIt = existingMeshDataMap.find(filename);
	if (existingMeshIt != existingMeshDataMap.end())
	{
		MeshData* existingMeshData = existingMeshIt->second;

		meshData->vertices = &existingMeshData->vertices;
		meshData->indices = &existingMeshData->indices;
		meshData->skeleton = &existingMeshData->skeleton;
		meshData->boundingBox = &existingMeshData->boudingBox;
		return true;
	}
	else
	{
		existingMeshDataMap[filename] = new MeshData();
	}

	if (!importer->Initialize(filepath.c_str(), -1, manager->GetIOSettings()))
	{
		throw new std::exception("FBX importer fucked up. filename probably wrong");
	}

	FbxScene* scene = FbxScene::Create(manager, "scene0");
	importer->Import(scene);

	//Automatically triangulate scene
	FbxGeometryConverter clsConverter(manager);
	clsConverter.Triangulate(scene, true);

	//This never seemed to do anything, left it here for future reference
	//scene->GetGlobalSettings().SetAxisSystem(FbxAxisSystem::DirectX);

	animEvaluator = scene->GetAnimationEvaluator();

	FbxNode* rootNode = scene->GetRootNode();
	int childNodeCount = rootNode->GetChildCount();

	MeshData* foundMeshData = existingMeshDataMap[filename];
	
	//Go through all skeleton nodes
	Skeleton* skeleton = &foundMeshData->skeleton;
	for (int i = 0; i < childNodeCount; i++)
	{
		ProcessSkeletonNodes(rootNode->GetChild(i), skeleton, -1);
	}

	//Go through all nodes
	for (int i = 0; i < childNodeCount; i++)
	{
		ProcessAllChildNodes(rootNode->GetChild(i), foundMeshData);
	}

	scene->Destroy();

	animEvaluator = nullptr;

	MeshData* newMeshData = existingMeshDataMap.find(filename)->second;
	assert(newMeshData->vertices.size() > 0);
	BoundingBox::CreateFromPoints(newMeshData->boudingBox, newMeshData->vertices.size(),
		&newMeshData->vertices.at(0).pos, sizeof(Vertex));

	//Set proxy data for new mesh daata
	meshData->vertices = &newMeshData->vertices;
	meshData->indices = &newMeshData->indices;
	meshData->skeleton = &newMeshData->skeleton;
	meshData->boundingBox = &newMeshData->boudingBox;

	return true;
}

void FBXLoader::ProcessAllChildNodes(FbxNode* node, MeshData* meshData)
{
	//Recursion for dealing with nodes in the heirarchy.
	int childNodeCount = node->GetChildCount();
	for (int i = 0; i < childNodeCount; i++)
	{
		ProcessAllChildNodes(node->GetChild(i), meshData);
	}

	FbxScene* scene = node->GetScene();
	std::string nodename = node->GetName();

	std::unordered_map<int, BoneWeights> boneWeightsMap;

	FbxAnimStack* animStack = nullptr;

	FbxMesh* mesh = node->GetMesh();
	if (mesh)
	{
		//Create animations for skeleton
		int numAnimStacks = scene->GetSrcObjectCount<FbxAnimStack>();
		for (int animStackIndex = 0; animStackIndex < numAnimStacks; animStackIndex++)
		{
			animStack = scene->GetSrcObject<FbxAnimStack>(animStackIndex);
			if (animStack)
			{
				std::string animName = animStack->GetName();
				meshData->skeleton.CreateAnimation(animName);
				meshData->skeleton.currentAnimation = animName;
			}
		}

		//WEIGHT AND BONE INDICES CODE
		const int deformerCount = mesh->GetDeformerCount(FbxDeformer::eSkin);
		for (int deformerIndex = 0; deformerIndex < deformerCount; deformerIndex++)
		{
			FbxSkin* skin = reinterpret_cast<FbxSkin*>(mesh->GetDeformer(deformerIndex, FbxDeformer::eSkin));
			if (!skin) continue;

			const int clusterCount = skin->GetClusterCount();
			for (int clusterIndex = 0; clusterIndex < clusterCount; clusterIndex++)
			{
				FbxCluster* cluster = skin->GetCluster(clusterIndex);

				//I think the Link is the joint
				std::string currentJointName = cluster->GetLink()->GetName();
				int currentJointIndex = meshData->skeleton.FindJointIndexByName(currentJointName);
				
				FbxAMatrix clusterMatrix, linkMatrix;
				cluster->GetTransformMatrix(clusterMatrix);
				cluster->GetTransformLinkMatrix(linkMatrix);

				{
					//Set inverse bind pose for joint
					FbxAMatrix bindposeInverseMatrix = linkMatrix.Inverse() * clusterMatrix;

					FbxQuaternion Q = bindposeInverseMatrix.GetQ();
					FbxVector4 T = bindposeInverseMatrix.GetT();

					XMVECTOR pos = XMVectorSet(T[0], T[1], T[2], 1.0f);
					XMVECTOR scale = XMVectorSet(1.f, 1.f, 1.f, 0.f);
					XMVECTOR rot = XMVectorSet(Q[0], Q[1], Q[2], Q[3]);

					XMMATRIX pose = XMMatrixAffineTransformation(scale,
						XMVectorSet(0.f, 0.f, 0.f, 1.f), rot, pos);
				
					meshData->skeleton.joints[currentJointIndex].inverseBindPose = pose;
					meshData->skeleton.joints[currentJointIndex].currentPose = pose;
				}

				const int vertexIndexCount = cluster->GetControlPointIndicesCount();
				for (int i = 0; i < vertexIndexCount; i++)
				{
					double weight = cluster->GetControlPointWeights()[i];
					weight = std::clamp(weight, 0.0, 1.0);

					int index = cluster->GetControlPointIndices()[i];

					if (boneWeightsMap[index].boneIndex.size() < BoneWeights::MAX_BONE_INDICES)
					{
						boneWeightsMap[index].boneIndex.push_back(currentJointIndex);
					}

					if (boneWeightsMap[index].weights.size() < BoneWeights::MAX_WEIGHTS)
					{
						boneWeightsMap[index].weights.push_back(weight);
					}
				}

				//Animation
				FbxInt nodeFlags = node->GetAllObjectFlags();
				if (nodeFlags & FbxPropertyFlags::eAnimated)
				{
					int numAnimStacks = scene->GetSrcObjectCount<FbxAnimStack>();
					for (int animStackIndex = 0; animStackIndex < numAnimStacks; animStackIndex++)
					{
						animStack = scene->GetSrcObject<FbxAnimStack>(animStackIndex);
						if (animStack)
						{
							std::string animName = animStack->GetName();
							meshData->skeleton.CreateAnimation(animName);
							meshData->skeleton.currentAnimation = animName;
							int numAnimLayers = animStack->GetMemberCount<FbxAnimLayer>();
							for (int animLayerIndex = 0; animLayerIndex < numAnimLayers; animLayerIndex++)
							{
								FbxAnimLayer* animLayer = animStack->GetMember<FbxAnimLayer>(animLayerIndex);
								if (animLayer)
								{
									//Link is the joint
									FbxNode* link = cluster->GetLink();
									std::string linkName = link->GetName();

									//@Todo: Feels like just getting one curve isn't the right thing here,
									//as it might be null if the skeleton only has translations.
									//I'm thinking if there are no rotation curves in the fbx, this won't work.
									FbxAnimCurveNode* curveNode = link->LclRotation.GetCurveNode(animLayer);
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
												FbxTime time = {};
												time.SetSecondDouble(keyTime);

												FbxAMatrix globalTransform = animEvaluator->GetNodeGlobalTransform(link, time);
												FbxQuaternion rot = globalTransform.GetQ();
												FbxVector4 scale = globalTransform.GetS();
												FbxVector4 pos = globalTransform.GetT();

												AnimFrame animFrame = {};
												animFrame.time = keyTime;

												animFrame.rot.x = rot[0];
												animFrame.rot.y = rot[1];
												animFrame.rot.z = rot[2];
												animFrame.rot.w = rot[3];

												animFrame.scale.x = 1.f;
												animFrame.scale.y = 1.f;
												animFrame.scale.z = 1.f;

												animFrame.pos.x = pos[0];
												animFrame.pos.y = pos[1];
												animFrame.pos.z = pos[2];

												meshData->skeleton.animations[animStack->GetName()].frames[currentJointIndex].push_back(animFrame);
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}

		//@Todo: materials for fbx files. Would need to figure out a workflow from Blender's materials
		//Material 
		//int materialCount = node->GetMaterialCount();
		//for (int materialIndex = 0; materialIndex < materialCount; materialIndex++)
		//{
		//	FbxSurfacePhong* material = (FbxSurfacePhong*)node->GetMaterial(materialIndex);
		//	FbxClassId surfaceID = material->GetClassId();
		//	FbxDouble3 ambient = material->Ambient.Get();
		//}

		//Array setup
		int numVerts = mesh->GetControlPointsCount();
		int vectorSize = numVerts * mesh->GetPolygonSize(0);
		assert((vectorSize % 3) == 0 && "FBX model isn't triangulated"); //This is a check to make sure the mesh is triangulated (in blender, Ctrl+T)

		//Geometry Elements
		FbxGeometryElementNormal* normals = mesh->GetElementNormal();
		FbxGeometryElementUV* uvs = mesh->GetElementUV();

		int polyIndexCounter = 0; //Used to index into normals and UVs on a per vertex basis
		int polyCount = mesh->GetPolygonCount();

		meshData->vertices.reserve(polyCount);
		meshData->indices.reserve(polyCount);

		//Main import loop
		for (int i = 0; i < polyCount; i++)
		{
			int polySize = mesh->GetPolygonSize(i);
			assert((polySize % 3) == 0 && "FBX model isn't triangulated");

			for (int j = 0; j < polySize; j++)
			{
				int index = mesh->GetPolygonVertex(i, j);

				//@Todo: index buffers aren't actually correct in the engine. they're essentially plain
				//Draw() calls at this point.
				//meshData.indices.push_back(index);

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
				if (boneWeightsMap.find(index) != boneWeightsMap.end())
				{
					BoneWeights* boneData = &boneWeightsMap.find(index)->second;
					if (boneData)
					{
						//There must be a way to merge the above cluster FBX code and the vertices.
						for (int i = 0; i < boneData->weights.size(); i++)
						{
							vert.weights[i] = boneData->weights[i];
						}

						for (int i = 0; i < boneData->boneIndex.size(); i++)
						{
							vert.boneIndices[i] = boneData->boneIndex[i];
						}
					}
				}

				//Indices checking if the engine gets it working...
				//if (!meshData.CheckDuplicateVertices(vert))
				//{
				//	meshData.vertices.push_back(vert);
				//}

				meshData->vertices.push_back(vert);
				meshData->indices.push_back(polyIndexCounter);

				polyIndexCounter++;
			}
		}

		assert(meshData->indices.size() % 3 == 0 && "Num of indices won't be matching vertices");
	}
}

void FBXLoader::ProcessSkeletonNodes(FbxNode* node, Skeleton* skeleton, int parentIndex)
{
	const int childCount = node->GetChildCount();
	for (int i = 0; i < childCount; i++)
	{
		FbxNode* child = node->GetChild(i);

		if (child->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::EType::eSkeleton)
		{
			Joint joint = {};
			joint.name = child->GetName();
			joint.parentIndex = parentIndex;
			skeleton->AddJoint(joint);

			Joint& addedJoint = skeleton->joints.back();

			ProcessSkeletonNodes(child, skeleton, addedJoint.index);
		}
	}
}

bool FBXLoader::ImportFracturedMesh(std::string filename, std::vector<MeshData>& meshDatas)
{
	std::string filepath = "Meshes/" + filename;

	if (filename.empty() || !std::filesystem::exists(filepath))
	{
		//set default model
		filename = "cube.fbx";
		filepath = "Meshes/cube.fbx";
	}

	if (!importer->Initialize(filepath.c_str(), -1, manager->GetIOSettings()))
	{
		throw new std::exception("FBX importer fucked up. filename probably wrong");
	}

	FbxScene* scene = FbxScene::Create(manager, "scene0");
	importer->Import(scene);

	//Automatically triangulate scene
	FbxGeometryConverter clsConverter(manager);
	clsConverter.Triangulate(scene, true);

	FbxNode* rootNode = scene->GetRootNode();
	int childNodeCount = rootNode->GetChildCount();

	meshDatas.resize(childNodeCount);

	//Go through all cells nodes
	for (int i = 0; i < childNodeCount; i++)
	{
		ProcessAllChildNodes(rootNode->GetChild(i), &meshDatas[i]);
	}

	scene->Destroy();

	return true;
}

MeshData* FBXLoader::FindMesh(std::string meshName)
{
	auto meshIt = existingMeshDataMap.find(meshName);
	if (meshIt == existingMeshDataMap.end())
	{
		return nullptr;
	}

	return meshIt->second;
}
