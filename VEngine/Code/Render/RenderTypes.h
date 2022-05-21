#pragma once
#include "vpch.h"
#include <DirectXCollision.h>
#include "Animation/AnimationStructures.h"

//@Todo: I think this file needs to be split up more. Make a RenderTypes folder and 
//figure out what can be seperated to minimise the number on #includes per file

using namespace DirectX;

struct MaterialShaderData;
struct MeshComponent;
struct Animation;
struct Material;

struct Vertex
{
	XMFLOAT3 pos;
	XMFLOAT3 normal;
	XMFLOAT2 uv;
	float weights[3];
	uint32_t boneIndices[4];
};

struct VRect
{
	int left;
	int top;
	int right;
	int bottom;
};

struct RastStates
{
	inline static const std::string solid = "solid";
	inline static const std::string noBackCull = "nobackcull";
	inline static const std::string wireframe = "wirefrmae";
	inline static const std::string shadow = "shadow";
};

struct BlendStates
{
	inline static const std::string null = "null";
	inline static const std::string Default = "default";
};

struct Line
{
	XMFLOAT3 p1;
	XMFLOAT3 p2;
};

//The actual data for each loaded mesh. Each loaded mesh file will have one of these per its filename.
struct MeshData
{
	typedef uint32_t indexDataType;

	std::vector<Vertex> vertices;
	std::vector<indexDataType> indices;

	//Base extents and offset will be the same for each mesh, fine to cache here.
	BoundingBox boudingBox;

	Skeleton skeleton;
};

//TextureData exists because serialisation needed texture representation that wasn't a Texture2D
//(Which is used by the renderer). This way you can pass TextureData in as a type instead of std::string
//for texture filanames and still keep all the previous widgets and functionality.
struct TextureData
{
	std::string filename;
};

//@Todo: not happy with the amount of serialisation errors rendertypes like TextureData/ShaderData have.
//Thinking just make everything std::strings (only cares about the filename anyway, they're only contained in
//structs so Qt can amke unique widgets for them per type) and add another std::function to call from
//Qt editor widgets and make a small autocomplete box or picker or whatever to compensate.

struct ShaderData
{
	std::string filename;
};

struct MeshComponentData
{
	std::string filename;
	MeshComponent* meshComponent = nullptr;
};

//A pointer structure to a MeshData struct in memory. Each rendered component will have one of these pointing
//to the mesh data on a per-filename basis.
struct MeshDataProxy
{
	std::vector<Vertex>* vertices = nullptr;
	std::vector<MeshData::indexDataType>* indices = nullptr;

	BoundingBox* boundingBox = nullptr;

	Skeleton* skeleton = nullptr;

	uint64_t GetVerticesByteWidth()
	{
		return (sizeof(Vertex) * vertices->size());
	}

	uint64_t GetIndicesByteWidth()
	{
		return (sizeof(MeshData::indexDataType) * indices->size());
	}

	void Clear()
	{
		vertices->clear();
		indices->clear();
	}

	bool CheckDuplicateVertices(Vertex& vert);

	//Duplicate checks for indices only return true if the index is present in the array
	//more than once. Eg. For {2, 1, 0}, {3, 1, 2}, 2 and 1 are the duplicates.
	bool CheckDuplicateIndices(MeshData::indexDataType index);
};

struct ShaderMatrices
{
	XMMATRIX model;
	XMMATRIX view;
	XMMATRIX proj;
	XMMATRIX mvp;
	XMMATRIX texMatrix;
	XMMATRIX lightMVP; //Light mvp is the shadowposition
	XMMATRIX lightViewProj; //This is for in Shadows.hlsl to push vertex pos into space

	void Create();
	void MakeTextureMatrix(Material* material);
	void MakeModelViewProjectionMatrix();
};

enum class LightType : int
{
	Directional = 0,
	Point = 1,
	Spot = 2
};

struct Light
{
	XMFLOAT4 position = XMFLOAT4(0.f, 0.f, 0.f, 1.f);
	XMFLOAT4 direction = XMFLOAT4(0.f, 0.f, 0.f, 0.f);
	XMFLOAT4 colour = XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	float spotAngle = 70.f;
	float intensity = 25.0f;
	int lightType = 0;
	int enabled = 1;
};

struct ShaderLights
{
	static const int MAX_LIGHTS = 32;

	XMFLOAT4 eyePosition = XMFLOAT4(0.f, 0.f, 0.f, 1.0);
	XMFLOAT4 globalAmbient = XMFLOAT4(0.f, 0.f, 0.f, 1.f);
	int numLights = 0;

	//Shadows for now are based on 1 directional light in the level. Set this to false to avoid the 
	//shadow map from building from an identity matrix when no directional lights exist in level.
	bool shadowsEnabled = false;

	int pad[2];

	Light lights[MAX_LIGHTS];
};

struct ShaderTimeData
{
	float deltaTime;
	float timeSinceStartup;
	float pad[2]; //D3D11 ConstantBuffers have to be a multiple of 16 bytes
};

struct ShaderMeshData
{
	XMFLOAT4 SH[9];
	XMFLOAT3 position = XMFLOAT3(0.f, 0.f, 0.f);
	float pad;
};

struct ShaderPostProcessData
{
	float exposure = 1.1f;
	float shoulderStrength = 0.22f;
	float linearStrength = 2.0f;
	float linearAngle = 0.01f;
	float toeStrenth = 0.2f;
	float toeNumerator = 0.011f;
	float toeDenominator = 0.33f;
	float linearWhitePointValue = 3.3f;
};

struct InstanceData
{
	XMMATRIX world = XMMatrixIdentity();
	XMFLOAT4 colour = XMFLOAT4(0.f, 0.f, 0.f, 1.f);
};
