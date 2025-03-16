import Asset.FBXLoader;
#define FBXSDK_SHARED //Needs to be defined for static linking
import <fbxsdk.h>;

using namespace fbxsdk;
using namespace DirectX;

void FBXLoader::Init()
{
	manager = FbxManager::Create();
	ioSetting = FbxIOSettings::Create(manager, IOSROOT);
	importer = FbxImporter::Create(manager, "");
}

void FBXLoader::ImportAsMesh(std::string filepath, MeshData& meshData)
{
	assert(std::filesystem::exists(filepath));

	if (!importer->Initialize(filepath.c_str(), -1, manager->GetIOSettings()))
	{
		throw new std::exception("FBX importer messed up. filename probably wrong");
	}

	FbxScene* scene = FbxScene::Create(manager, "scene0");
	importer->Import(scene);

	//Automatically triangulate scene
	FbxGeometryConverter clsConverter(manager);
	clsConverter.Triangulate(scene, true);

	//This never seemed to do anything, left it here for future reference
	//scene->GetGlobalSettings().SetAxisSystem(FbxAxisSystem::DirectX);

	const std::string meshFilename =
		VString::GetSubStringAtFoundOffset(filepath, AssetBaseFolders::fbxFiles);

	FbxNode* rootNode = scene->GetRootNode();
	int childNodeCount = rootNode->GetChildCount();

	//Go through all skeleton nodes
	for (int i = 0; i < childNodeCount; i++)
	{
		ProcessSkeletonNodes(rootNode->GetChild(i), meshData.skeleton, Joint::ROOT_JOINT_INDEX);
	}

	//Go through all nodes
	for (int i = 0; i < childNodeCount; i++)
	{
		ProcessAllChildNodes(meshFilename, rootNode->GetChild(i), meshData);
	}

	scene->Destroy();

	assert(meshData.vertices.size() > 0 && "Nothing probably selected on fbx export in DCC.");
	BoundingBox::CreateFromPoints(meshData.boundingBox, meshData.vertices.size(),
		&meshData.vertices.at(0).pos, sizeof(Vertex));
}

std::map<std::string, Animation> FBXLoader::ImportAsAnimation(const std::string filepath, const std::string filename)
{
	assert(std::filesystem::exists(filepath));

	if (!importer->Initialize(filepath.c_str(), -1, manager->GetIOSettings()))
	{
		throw new std::exception("FBX importer fucked up. filename probably wrong");
	}

	FbxScene* scene = FbxScene::Create(manager, "scene0");
	importer->Import(scene);

	FbxAnimEvaluator* animEvaluator = scene->GetAnimationEvaluator();

	FbxNode* rootNode = scene->GetRootNode();

	const int childNodeCount = rootNode->GetChildCount();

	Skeleton skeleton;
	std::string animationName;

	//Go through all skeleton nodes
	for (int i = 0; i < childNodeCount; i++)
	{
		constexpr int rootBoneParentIndex = -1;
		ProcessSkeletonNodes(rootNode->GetChild(i), skeleton, rootBoneParentIndex);
	}

	for (int i = 0; i < childNodeCount; i++)
	{
		FbxNode* childNode = rootNode->GetChild(i);

		FbxMesh* mesh = childNode->GetMesh();
		if (mesh == nullptr) continue;

		const int animStackCount = scene->GetSrcObjectCount<FbxAnimStack>();
		for (int animStackIndex = 0; animStackIndex < animStackCount; animStackIndex++)
		{
			FbxAnimStack* animStack = scene->GetSrcObject<FbxAnimStack>(animStackIndex);
			if (animStack)
			{
				//SUPER IMPORTANT to set the current FbxAnimStack. Otherwise, the SDK just grabs the first global
				//animation in the file and works the Deformers, Clusters Links and Curves from that.
				//Was hard to find this. Not much information on it, didn't see it in the official samples.
				//https://www.reddit.com/r/gamedev/comments/tmery7/fbx_sdk_evaluatelocaltransform/
				//https://blender.stackexchange.com/questions/142783/can-i-export-multiple-objects-with-individual-animations-to-fbx
				scene->SetCurrentAnimationStack(animStack);

				animationName = animStack->GetName();
				skeleton.CreateAnimation(animationName);
			}

			const int deformerCount = mesh->GetDeformerCount(FbxDeformer::eSkin);
			for (int deformerIndex = 0; deformerIndex < deformerCount; deformerIndex++)
			{
				FbxSkin* skin = reinterpret_cast<FbxSkin*>(mesh->GetDeformer(deformerIndex, FbxDeformer::eSkin));
				if (!skin) continue;

				const int clusterCount = skin->GetClusterCount();
				for (int clusterIndex = 0; clusterIndex < clusterCount; clusterIndex++)
				{
					FbxCluster* cluster = skin->GetCluster(clusterIndex);

					//'Link' is the joint
					std::string currentJointName = cluster->GetLink()->GetName();
					const int currentJointIndex = skeleton.FindJointIndexByName(currentJointName);

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

						skeleton.GetJoints()[currentJointIndex].inverseBindPose = pose;
						skeleton.GetJoints()[currentJointIndex].currentPose = pose;
					}

					FbxInt nodeFlags = rootNode->GetAllObjectFlags();
					if (nodeFlags & FbxPropertyFlags::eAnimated)
					{
						if (animStack)
						{
							FbxAnimLayer* animLayer = animStack->GetMember<FbxAnimLayer>();
							std::string animLayerName = animLayer->GetName();

							//Link is the joint
							FbxNode* link = cluster->GetLink();
							std::string linkName = link->GetName();

							//@Todo: Feels like just getting one curve isn't the right thing here,
							//as it might be null if the skeleton only has translations.
							//I'm thinking if there are no rotation curves in the fbx, this won't work.
							FbxAnimCurveNode* curveNode = link->LclRotation.GetCurveNode(animLayer);
							if (curveNode)
							{
								const int numCurveNodes = curveNode->GetCurveCount(0);
								for (int curveIndex = 0; curveIndex < numCurveNodes; curveIndex++)
								{
									FbxAnimCurve* animCurve = curveNode->GetCurve(curveIndex);
									std::string animCurveName = animCurve->GetName();

									std::vector<AnimFrame> animFrames;

									const int keyCount = animCurve->KeyGetCount();
									for (int keyIndex = 0; keyIndex < keyCount; keyIndex++)
									{
										//Keys are the keyframes into the animation
										const double keyTime = animCurve->KeyGet(keyIndex).GetTime().GetSecondDouble();
										FbxTime time = {};
										time.SetSecondDouble(keyTime);

										const FbxAMatrix globalTransform = animEvaluator->GetNodeGlobalTransform(link, time);
										const FbxQuaternion rot = globalTransform.GetQ();
										const FbxVector4 scale = globalTransform.GetS();
										const FbxVector4 pos = globalTransform.GetT();

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

										animFrames.emplace_back(animFrame);
									}

									Animation& animation = skeleton.GetAnimation(animationName);
									animation.AddFrame(currentJointIndex, animFrames);
								}
							}
						}
					}
				}
			}
		}
	}

	return skeleton.GetAnimations();
}

void FBXLoader::ProcessAllChildNodes(std::string fbxFilename, FbxNode* node, MeshData& meshData)
{
	// Recursion for handling child nodes.
	ProcessChildNodes(fbxFilename, node, meshData);

	// Process the mesh associated with this node.
	ProcessMesh(node, meshData, fbxFilename);
}

void FBXLoader::ProcessChildNodes(std::string fbxFilename, FbxNode* node, MeshData& meshData)
{
	int childNodeCount = node->GetChildCount();
	for (int i = 0; i < childNodeCount; i++)
	{
		ProcessAllChildNodes(fbxFilename, node->GetChild(i), meshData);
	}
}

void FBXLoader::ProcessMesh(FbxNode* node, MeshData& meshData, const std::string& fbxFilename)
{
	FbxMesh* mesh = node->GetMesh();
	if (!mesh)
		return;

	std::unordered_map<int, BoneWeights> boneWeightsMap;
	ProcessBoneWeights(mesh, meshData, boneWeightsMap);
	ProcessMaterials(node, fbxFilename);
	ProcessVertices(mesh, meshData, boneWeightsMap);
}

void FBXLoader::ProcessBoneWeights(FbxMesh* mesh, MeshData& meshData, std::unordered_map<int, BoneWeights>& boneWeightsMap)
{
	const int deformerCount = mesh->GetDeformerCount(FbxDeformer::eSkin);
	for (int deformerIndex = 0; deformerIndex < deformerCount; deformerIndex++)
	{
		FbxSkin* skin = reinterpret_cast<FbxSkin*>(mesh->GetDeformer(deformerIndex, FbxDeformer::eSkin));
		if (!skin) continue;

		const int clusterCount = skin->GetClusterCount();
		for (int clusterIndex = 0; clusterIndex < clusterCount; clusterIndex++)
		{
			ProcessCluster(skin->GetCluster(clusterIndex), mesh, meshData, boneWeightsMap);
		}
	}
}

void FBXLoader::ProcessCluster(FbxCluster* cluster, FbxMesh* mesh, MeshData& meshData, std::unordered_map<int, BoneWeights>& boneWeightsMap)
{
	// Get current joint name and index
	std::string currentJointName = cluster->GetLink()->GetName();
	int currentJointIndex = meshData.skeleton.FindJointIndexByName(currentJointName);

	FbxAMatrix clusterMatrix, linkMatrix;
	cluster->GetTransformMatrix(clusterMatrix);
	cluster->GetTransformLinkMatrix(linkMatrix);

	// Set inverse bind pose for joint
	SetInverseBindPose(meshData, currentJointIndex, linkMatrix, clusterMatrix);

	const int vertexIndexCount = cluster->GetControlPointIndicesCount();
	for (int i = 0; i < vertexIndexCount; i++)
	{
		ProcessVertexWeights(cluster, i, currentJointIndex, boneWeightsMap);
	}
}

void FBXLoader::SetInverseBindPose(MeshData& meshData, int currentJointIndex, const FbxAMatrix& linkMatrix, const FbxAMatrix& clusterMatrix)
{
	FbxAMatrix bindposeInverseMatrix = linkMatrix.Inverse() * clusterMatrix;

	FbxQuaternion Q = bindposeInverseMatrix.GetQ();
	FbxVector4 T = bindposeInverseMatrix.GetT();

	XMVECTOR pos = XMVectorSet(T[0], T[1], T[2], 1.0f);
	XMVECTOR scale = XMVectorSet(1.f, 1.f, 1.f, 0.f);
	XMVECTOR rot = XMVectorSet(Q[0], Q[1], Q[2], Q[3]);

	XMMATRIX pose = XMMatrixAffineTransformation(scale, XMVectorSet(0.f, 0.f, 0.f, 1.f), rot, pos);

	meshData.skeleton.GetJoint(currentJointIndex).inverseBindPose = pose;
	meshData.skeleton.GetJoint(currentJointIndex).currentPose = pose;
}

void FBXLoader::ProcessVertexWeights(FbxCluster* cluster, int i, int currentJointIndex, std::unordered_map<int, BoneWeights>& boneWeightsMap)
{
	double weight = cluster->GetControlPointWeights()[i];
	weight = std::clamp(weight, 0.0, 1.0);
	int index = cluster->GetControlPointIndices()[i];

	if (boneWeightsMap[index].boneIndex.size() < BoneWeights::MAX_BONE_INDICES)
	{
		boneWeightsMap[index].boneIndex.emplace_back(currentJointIndex);
	}

	if (boneWeightsMap[index].weights.size() < BoneWeights::MAX_WEIGHTS)
	{
		boneWeightsMap[index].weights.emplace_back(weight);
	}
}

void FBXLoader::ProcessMaterials(FbxNode* node, const std::string& fbxFilename)
{
	const int materialCount = node->GetMaterialCount();
	for (int materialIndex = 0; materialIndex < materialCount; materialIndex++)
	{
		auto FBXMaterial = (FbxSurfacePhong*)node->GetMaterial(materialIndex);
		ProcessMaterial(FBXMaterial, fbxFilename);
	}
}

void FBXLoader::ProcessMaterial(FbxSurfacePhong* fbxMaterial, const std::string& fbxFilename)
{
	MaterialShaderData shaderData;
	shaderData.ambient = FBXDouble3ToXMFloat3(fbxMaterial->Ambient.Get());
	shaderData.emissive = FBXDouble3ToXMFloat3(fbxMaterial->Emissive.Get());
	shaderData.diffuse = FBXDouble3ToXMFloat3(fbxMaterial->Diffuse.Get());
	shaderData.specular = FBXDouble3ToXMFloat3(fbxMaterial->Specular.Get());

	//@Todo: No idea how to get metallic & roughness GGX material data from blender.
	//(Don't think PBR material export values exist in FBX SDK. Maybe you'd have to define your
	//own user values in DCC? Here's some dudes talking about it.
	//https://forums.autodesk.com/t5/fbx-forum/pbr-materials/td-p/7418493)
	//@Todo: textures (really just texture filenames)
	//@Todo: No idea how transparency applies to each material per colour property in DCC.

	Material engineMaterial("test.png", "Default");
	engineMaterial.GetMaterialShaderData() = shaderData;

	const std::string materialName = AssetBaseFolders::material + VString::GetSubStringBeforeFoundOffset(fbxFilename, ".fbx") + fbxMaterial->GetName() + AssetFileExtensions::material;
	assert(!std::filesystem::exists(materialName));

	Serialiser s(materialName, OpenMode::Out);
	Properties materialProps = engineMaterial.GetProps();
	s.Serialise(materialProps);

	Log("Material file [%s] created from mesh import [%s].", materialName.c_str(), fbxFilename.c_str());
}

void FBXLoader::ProcessVertices(FbxMesh* mesh, MeshData& meshData, const std::unordered_map<int, BoneWeights>& boneWeightsMap)
{
	int numVerts = mesh->GetControlPointsCount();
	int vectorSize = numVerts * mesh->GetPolygonSize(0);
	assert((vectorSize % 3) == 0 && "FBX model isn't triangulated");

	int polyIndexCounter = 0;
	const int triangleCount = mesh->GetPolygonCount();
	meshData.vertices.reserve(triangleCount);

	//Blender exporting FBX to right-handed coordinate system means you have to:
	// - invert the vertex's Z by -1 (-1.0 * vertex.z)
	// - invert the texture UV u coordinate by (1.0 - uv.v) [can be done in shader too]
	// - flip the vertex order (from v0, v1, v2 to v0, v2, v1)
	// Also optional to rotate the model around a bit (apply a -90 rotation on the x-axis, apply the rotation
	//then rotate 90 deg on the x-axis), I think Unity does this approach internally.

	for (int i = 0; i < triangleCount; i++)
	{
		ProcessTriangle(mesh, i, polyIndexCounter, meshData, boneWeightsMap);
	}

	FlipVertexFaceOrder(meshData);
}

void FBXLoader::ProcessTriangle(FbxMesh* mesh, int triangleIndex, int& polyIndexCounter, MeshData& meshData, const std::unordered_map<int, BoneWeights>& boneWeightsMap)
{
	const int triangleSize = mesh->GetPolygonSize(triangleIndex);
	assert((triangleSize % 3) == 0 && "FBX model isn't triangulated");

	for (int j = 0; j < triangleSize; j++)
	{
		const int index = mesh->GetPolygonVertex(triangleIndex, j);

		Vertex vert = {};
		auto controlPoint = mesh->GetControlPointAt(index);
		vert.pos.x = controlPoint.mData[0];
		vert.pos.y = controlPoint.mData[1];
		vert.pos.z = -1.0f * controlPoint.mData[2];

		ReadUVs(mesh, index, polyIndexCounter, vert.uv);
		vert.uv.y = 1.0f - vert.uv.y;

		ReadVertexColours(mesh, index, polyIndexCounter, vert.colour);
		ReadNormal(mesh, index, polyIndexCounter, vert.normal);

		vert.normal.z = -1.0f * vert.normal.z;

		// Bone Weights
		if (boneWeightsMap.find(index) != boneWeightsMap.end())
		{
			const BoneWeights& boneData = boneWeightsMap.find(index)->second;

			for (int weightIndex = 0; weightIndex < boneData.weights.size(); weightIndex++)
			{
				vert.weights[weightIndex] = boneData.weights[weightIndex];
			}

			for (int boneIndex = 0; boneIndex < boneData.boneIndex.size(); boneIndex++)
			{
				vert.boneIndices[boneIndex] = boneData.boneIndex[boneIndex];
			}
		}

		meshData.vertices.emplace_back(vert);
		polyIndexCounter++;
	}
}

void FBXLoader::FlipVertexFaceOrder(MeshData& meshData)
{
	for (int i = 0; i < meshData.vertices.size() / 3; i++)
	{
		const int index1 = i * 3 + 1;
		const int index2 = i * 3 + 2;
		std::swap(meshData.vertices[index1], meshData.vertices[index2]);
	}

	// Also set tangents
	for (int i = 0; i < meshData.vertices.size() / 3; i++)
	{
		const int index0 = i * 3;
		const int index1 = i * 3 + 1;
		const int index2 = i * 3 + 2;

		Vertex* verts[3] = { &meshData.vertices[index0], &meshData.vertices[index1], &meshData.vertices[index2] };
		SetTangents(verts);
	}
}

void FBXLoader::SetTangents(Vertex* verts[3])
{
	const XMFLOAT3 edge1 = VMath::Float3Subtract(verts[1]->pos, verts[0]->pos);
	const XMFLOAT3 edge2 = VMath::Float3Subtract(verts[2]->pos, verts[0]->pos);

	const XMFLOAT2 deltaUV1 = VMath::Float2Subtract(verts[1]->uv, verts[0]->uv);
	const XMFLOAT2 deltaUV2 = VMath::Float2Subtract(verts[2]->uv, verts[0]->uv);

	const float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	XMFLOAT3 tangent1{};
	tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

	// Assign tangent to all verts
	for (int vIndex = 0; vIndex < 3; ++vIndex)
	{
		verts[vIndex]->tangent = tangent1;
	}
}

void FBXLoader::ProcessSkeletonNodes(FbxNode* node, Skeleton& skeleton, int parentIndex)
{
	const int childCount = node->GetChildCount();
	for (int i = 0; i < childCount; i++)
	{
		FbxNode* child = node->GetChild(i);

		if (child->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::EType::eSkeleton)
		{
			Joint joint;
			joint.SetName(child->GetName());
			joint.parentIndex = parentIndex;
			skeleton.AddJoint(joint);

			Joint& addedJoint = skeleton.GetJoints().back();

			ProcessSkeletonNodes(child, skeleton, addedJoint.index);
		}
	}
}

void FBXLoader::ImportFracturedMesh(std::string filename, std::vector<MeshData>& meshDatas)
{
	std::string filepath = AssetBaseFolders::fbxFiles + filename;

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

	const std::string meshFilename =
		VString::GetSubStringAtFoundOffset(filepath, AssetBaseFolders::mesh);

	//Go through all cells nodes
	for (int i = 0; i < childNodeCount; i++)
	{
		ProcessAllChildNodes(meshFilename, rootNode->GetChild(i), meshDatas[i]);
	}

	scene->Destroy();
}

//Took this function and ReadUVs() from a tutorial but forgot where from.
void FBXLoader::ReadNormal(FbxMesh* inMesh, int inCtrlPointIndex, int inVertexCounter, XMFLOAT3& outNormal)
{
	if (inMesh->GetElementNormalCount() < 1) { throw std::exception("Invalid Normal Number"); }

	FbxGeometryElementNormal* vertexNormal = inMesh->GetElementNormal(0);

	switch (vertexNormal->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
		switch (vertexNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			outNormal.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inCtrlPointIndex).mData[0]);
			outNormal.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inCtrlPointIndex).mData[1]);
			outNormal.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inCtrlPointIndex).mData[2]);
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexNormal->GetIndexArray().GetAt(inCtrlPointIndex);
			outNormal.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
			outNormal.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
			outNormal.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
		}
		break;
		default:
			throw std::exception("Invalid Reference");
		}
		break;
	case FbxGeometryElement::eByPolygonVertex:
		switch (vertexNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			outNormal.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inVertexCounter).mData[0]);
			outNormal.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inVertexCounter).mData[1]);
			outNormal.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inVertexCounter).mData[2]);
		}
		break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexNormal->GetIndexArray().GetAt(inVertexCounter);
			outNormal.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
			outNormal.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
			outNormal.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
		}
		break;
		default: throw std::exception("Invalid Reference");
		}

		break;
	}
}

void FBXLoader::ReadUVs(FbxMesh* inMesh, int inCtrlPointIndex, int inVertexCounter, XMFLOAT2& outUVs)
{
	if (inMesh->GetElementNormalCount() < 1) { throw std::exception("Invalid Normal Number"); }

	FbxGeometryElementUV* vertexUVs = inMesh->GetElementUV(0);

	switch (vertexUVs->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
		switch (vertexUVs->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			outUVs.x = static_cast<float>(vertexUVs->GetDirectArray().GetAt(inCtrlPointIndex).mData[0]);
			outUVs.y = static_cast<float>(vertexUVs->GetDirectArray().GetAt(inCtrlPointIndex).mData[1]);
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexUVs->GetIndexArray().GetAt(inCtrlPointIndex);
			outUVs.x = static_cast<float>(vertexUVs->GetDirectArray().GetAt(index).mData[0]);
			outUVs.y = static_cast<float>(vertexUVs->GetDirectArray().GetAt(index).mData[1]);
		}
		break;
		default:
			throw std::exception("Invalid Reference");
		}
		break;
	case FbxGeometryElement::eByPolygonVertex:
		switch (vertexUVs->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			outUVs.x = static_cast<float>(vertexUVs->GetDirectArray().GetAt(inVertexCounter).mData[0]);
			outUVs.y = static_cast<float>(vertexUVs->GetDirectArray().GetAt(inVertexCounter).mData[1]);
		}
		break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexUVs->GetIndexArray().GetAt(inVertexCounter);
			outUVs.x = static_cast<float>(vertexUVs->GetDirectArray().GetAt(index).mData[0]);
			outUVs.y = static_cast<float>(vertexUVs->GetDirectArray().GetAt(index).mData[1]);
		}
		break;
		default: throw std::exception("Invalid Reference");
		}

		break;
	}
}

void FBXLoader::ReadVertexColours(FbxMesh* inMesh, int inCtrlPointIndex, int inVertexCounter, XMFLOAT4& outColour)
{
	FbxGeometryElementVertexColor* vertexColour = inMesh->GetElementVertexColor(0);

	if (vertexColour == nullptr)
	{
		outColour = XMFLOAT4(1.f, 1.f, 1.f, 1.f);
		return;
	}

	//Colours formatted to RGBA
	switch (vertexColour->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
		switch (vertexColour->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			outColour.x = static_cast<float>(vertexColour->GetDirectArray().GetAt(inCtrlPointIndex).mRed);
			outColour.y = static_cast<float>(vertexColour->GetDirectArray().GetAt(inCtrlPointIndex).mGreen);
			outColour.z = static_cast<float>(vertexColour->GetDirectArray().GetAt(inCtrlPointIndex).mBlue);
			//@Todo: alpha vertex colours in DCC. Might cause trouble in engine?
			//outColour.w = static_cast<float>(vertexColour->GetDirectArray().GetAt(inCtrlPointIndex).mAlpha);
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexColour->GetIndexArray().GetAt(inCtrlPointIndex);
			outColour.x = static_cast<float>(vertexColour->GetDirectArray().GetAt(index).mRed);
			outColour.y = static_cast<float>(vertexColour->GetDirectArray().GetAt(index).mGreen);
			outColour.z = static_cast<float>(vertexColour->GetDirectArray().GetAt(index).mBlue);
		}
		break;
		default:
			throw std::exception("Invalid Reference");
		}
		break;
	case FbxGeometryElement::eByPolygonVertex:
		switch (vertexColour->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			outColour.x = static_cast<float>(vertexColour->GetDirectArray().GetAt(inVertexCounter).mRed);
			outColour.y = static_cast<float>(vertexColour->GetDirectArray().GetAt(inVertexCounter).mGreen);
			outColour.z = static_cast<float>(vertexColour->GetDirectArray().GetAt(inVertexCounter).mBlue);
		}
		break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexColour->GetIndexArray().GetAt(inVertexCounter);
			outColour.x = static_cast<float>(vertexColour->GetDirectArray().GetAt(index).mRed);
			outColour.y = static_cast<float>(vertexColour->GetDirectArray().GetAt(index).mGreen);
			outColour.z = static_cast<float>(vertexColour->GetDirectArray().GetAt(index).mBlue);
		}
		break;
		default: throw std::exception("Invalid Reference");
		}

		break;
	}
}

XMFLOAT4 FBXLoader::FBXDouble3ToXMFloat3(const fbxsdk::FbxDouble3& fbxDouble)
{
	return XMFLOAT4(fbxDouble.mData[0], fbxDouble.mData[1], fbxDouble.mData[2], 1.f);
}

std::vector<XMFLOAT3> FBXLoader::ProcessControlPoints(FbxMesh* currMesh)
{
	const int ctrlPointCount = currMesh->GetControlPointsCount();
	std::vector<XMFLOAT3> controlPoints;
	for (int i = 0; i < ctrlPointCount; i++)
	{
		XMFLOAT3 currPosition = {};
		currPosition.x = static_cast<float>(currMesh->GetControlPointAt(i).mData[0]);
		currPosition.y = static_cast<float>(currMesh->GetControlPointAt(i).mData[1]);
		currPosition.z = static_cast<float>(currMesh->GetControlPointAt(i).mData[2]);
		controlPoints.emplace_back(currPosition);
	}

	return controlPoints;
}
