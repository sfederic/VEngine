#pragma once
#include <vector>
#include <string>
#include <DirectXMath.h>
#include <DirectXCollision.h>

using namespace DirectX;

struct MaterialShaderData;
struct MeshComponent;

struct Vertex
{
	XMFLOAT3 pos;
	XMFLOAT3 normal;
	XMFLOAT2 uv;
};

//The actual data for each loaded mesh. Each loaded mesh file will have one of these per its filename.
struct MeshData
{
	typedef uint32_t indexDataType;

	std::vector<Vertex> vertices;
	std::vector<indexDataType> indices;
};

//TextureData exists because serialisation needed texture representation that wasn't a Texture2D
//(Which is used by the renderer). This way you can pass TextureData in as a type instead of std::string
//for texture filanames and still keep all the previous widgets and functionality.
struct TextureData
{
	std::string filename;
};

//TODO: there'll be a nicer way to do these and name these. Maybe try adding in another argument to
//the 'change' std::function in Properties.
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

	bool CheckDuplicateVertices(Vertex& vert)
	{
		auto pos = XMLoadFloat3(&vert.pos);

		const int size = vertices->size();
		for (int i = 0; i < size; i++)
		{
			XMVECTOR p = XMLoadFloat3(&vertices->at(i).pos);
			if (XMVector3Equal(p, pos))
			{
				return true;
			}
		}

		return false;
	}

	//Duplicate checks for indices only return true if the index is present in the array
	//more than once. Eg. For {2, 1, 0}, {3, 1, 2}, 2 and 1 are the duplicates.
	bool CheckDuplicateIndices(MeshData::indexDataType index)
	{
		int duplicateCounter = 0;

		for (int i = 0; i < indices->size(); i++)
		{
			if (index == indices->at(i))
			{
				duplicateCounter++;
				if (duplicateCounter >= 2)
				{
					return true;
				}
			}
		}

		return false;
	}
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
	void MakeTextureMatrix(MaterialShaderData* shaderData);
	void MakeModelViewProjectionMatrix();
};

enum class LightType : int
{
	Directional = 0,
	Point = 1,
	Spot = 2
};

//Still the best tutorial for lighting in D3D11.
//REF:https://www.3dgep.com/texturing-lighting-directx-11/#Attenuation
struct Light
{
	XMFLOAT4 position = XMFLOAT4(0.f, 0.f, 0.f, 1.f);
	XMFLOAT4 direction = XMFLOAT4(0.f, 0.f, 0.f, 0.f);
	XMFLOAT4 colour = XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	float spotAngle = 90.f;
	float constantAtten = 0.f;
	float linearAtten = 0.f;
	float quadraticAtten = 0.f;
	int lightType = 0;
	int enabled = 1;
	float pad[2];
};

struct ShaderLights
{
	static const int MAX_LIGHTS = 32;

	XMFLOAT4 eyePosition = XMFLOAT4(0.f, 0.f, 0.f, 1.0);
	XMFLOAT4 globalAmbience = XMFLOAT4(0.f, 0.f, 0.f, 1.f);
	int numLights = 0;
	int pad[3];

	Light lights[MAX_LIGHTS];
};

struct InstanceData
{
	XMMATRIX world;
};
