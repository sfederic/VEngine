#include "Actor.h"
#include "RenderSystem.h"
#include "MathHelpers.h"
#include "Buffer.h"
#include "RasterizerState.h"
#include "ShaderResourceView.h"
#include "Sampler.h"
#include "Texture.h"

Actor::Actor()
{

}

void Actor::Tick(float deltaTime)
{
}

void Actor::Start()
{
}

Properties Actor::GetSaveProps()
{
	Properties props;
	props.Add("Name", &name);
	props.Add("Position", &transform.position);
	props.Add("Scale", &transform.scale);
	props.Add("RotQuat", &transform.quatRotation);
	return props;
}

Properties Actor::GetEditorProps()
{
	Properties props;
	props.Add("Render", &bRender);
	return props;
}

XMVECTOR Actor::GetPositionVector()
{
	return XMLoadFloat3(&transform.position);
}

XMFLOAT3 Actor::GetPositionFloat3()
{
	return transform.position;
}

void Actor::SetPosition(XMVECTOR v)
{
	XMStoreFloat3(&transform.position, v);
}

void Actor::SetPosition(float x, float y, float z)
{
	transform.position = XMFLOAT3(x, y, z);
}

void Actor::SetPosition(XMFLOAT3 pos)
{
	transform.position = pos;
}

void Actor::SetRotation(XMVECTOR quaternion)
{
	XMStoreFloat4(&transform.quatRotation, quaternion);
}

void Actor::SetRotation(XMVECTOR axis, float angle)
{
	if (XMVector3Equal(XMVectorZero(), axis))
	{
		transform.quatRotation = XMFLOAT4(0.f, 0.f, 0.f, 1.f);
		return;
	}

	float andleRadians = XMConvertToRadians(angle);
	XMStoreFloat4(&transform.quatRotation, XMQuaternionRotationAxis(axis, andleRadians));
}

void Actor::SetRotation(float pitch, float yaw, float roll)
{
	transform.euler = XMFLOAT3(pitch, yaw, roll);

	float pitchRadians = XMConvertToRadians(pitch);
	float yawRadians = XMConvertToRadians(yaw);
	float rollRadians = XMConvertToRadians(roll);
	XMStoreFloat4(&transform.quatRotation, XMQuaternionRotationRollPitchYaw(pitchRadians, yawRadians, rollRadians));
}

void Actor::SetRotation(XMFLOAT3 euler)
{
	transform.euler = euler;

	float pitchRadians = XMConvertToRadians(euler.x);
	float yawRadians = XMConvertToRadians(euler.y);
	float rollRadians = XMConvertToRadians(euler.z);
	XMStoreFloat4(&transform.quatRotation, XMQuaternionRotationRollPitchYaw(pitchRadians, yawRadians, rollRadians));
}

XMFLOAT4 Actor::GetRotationQuat()
{
	return transform.quatRotation;
}

XMMATRIX Actor::GetTransformationMatrix()
{
	return XMMatrixAffineTransformation(
		XMLoadFloat3(&transform.scale),
		XMVectorSet(0.f, 0.f, 0.f, 1.f),
		XMLoadFloat4(&transform.quatRotation),
		XMLoadFloat3(&transform.position)
	);
}

//This function traverses up the parent hierarchy, saves the new world matrix on the current actor
//and also returns that matrix in world space.
//It's not a nice looking function because of the recursion. There's room to clean it up.
XMMATRIX Actor::DecomposeTransformationMatrix(Actor* actor)
{
	if (actor == nullptr)
	{
		actor = this;
	}

	if (actor->parent == nullptr || actor->isRoot)
	{
		XMMATRIX actorMatrix = actor->GetTransformationMatrix();
		actor->transform.Decompose(actorMatrix);

		//Set previous 
		actor->transform.oldPosition = actor->transform.position;
		actor->transform.oldScale = actor->transform.scale;
		actor->transform.oldQuatRotation = actor->transform.quatRotation;

		return actorMatrix;
	}

	XMMATRIX toParent = actor->parent->GetTransformationMatrix();
	XMMATRIX parentToRoot = XMMatrixIdentity();
	if (!actor->parent->isRoot)
	{
		parentToRoot = DecomposeTransformationMatrix(actor->parent);
	}

	XMMATRIX toRoot = toParent * parentToRoot;
	XMMATRIX result = actor->GetTransformationMatrix() * toRoot;

	Transform tempTransform;
	tempTransform.Decompose(result);

	if (VecEqual(XMLoadFloat3(&actor->transform.oldPosition), XMLoadFloat3(&tempTransform.position)))
	{
		if (VecEqual(XMLoadFloat4(&actor->transform.oldQuatRotation), XMLoadFloat4(&tempTransform.quatRotation)))
		{
			if (VecEqual(XMLoadFloat3(&actor->transform.oldScale), XMLoadFloat3(&tempTransform.scale)))
			{
				return actor->GetTransformationMatrix(); // Don't decompose matrix if old properties are equal to current		
			}
		}
	}

	actor->transform.Decompose(result);

	//Set previous again (first one is a fall through for root actors)
	actor->transform.oldPosition = actor->transform.position;
	actor->transform.oldScale = actor->transform.scale;
	actor->transform.oldQuatRotation = actor->transform.quatRotation;

	return result;
}

XMFLOAT3 Actor::GetPitchYawRoll()
{
	return PitchYawRollFromMatrix(GetTransformationMatrix());
}

XMFLOAT3 Actor::GetScale()
{
	return transform.scale;
}

void Actor::SetScale(float x, float y, float z)
{
	transform.scale = XMFLOAT3(x, y, z);
}

void Actor::SetScale(XMVECTOR scale)
{
	XMStoreFloat3(&transform.scale, scale);
}

void Actor::SetScale(XMFLOAT3 scale)
{
	transform.scale = scale;
}

XMVECTOR Actor::GetForwardVector()
{
	XMMATRIX m = XMMatrixAffineTransformation(
		XMLoadFloat3(&transform.scale),
		XMVectorSet(0.f, 0.f, 0.f, 1.f),
		XMLoadFloat4(&transform.quatRotation),
		XMLoadFloat3(&transform.position)
	);

	return m.r[2];
}

XMVECTOR Actor::GetRightVector()
{
	XMMATRIX m = XMMatrixAffineTransformation(
		XMLoadFloat3(&transform.scale),
		XMVectorSet(0.f, 0.f, 0.f, 1.f),
		XMLoadFloat4(&transform.quatRotation),
		XMLoadFloat3(&transform.position)
	);

	return m.r[0];
}

XMVECTOR Actor::GetUpVector()
{
	XMMATRIX m = XMMatrixAffineTransformation(
		XMLoadFloat3(&transform.scale),
		XMVectorSet(0.f, 0.f, 0.f, 1.f),
		XMLoadFloat4(&transform.quatRotation),
		XMLoadFloat3(&transform.position)
	);

	return m.r[1];
}

void Actor::Move(float d, XMVECTOR direction)
{
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR loc = GetPositionVector();
	loc = XMVectorMultiplyAdd(s, direction, loc);
	SetPosition(loc);
}

ActorSystem::ActorSystem()
{

}

ActorSystem* Actor::GetActorSystem()
{
	if (linkedActorSystem)
	{
		return linkedActorSystem;
	}

	return nullptr;
}

void Actor::Destroy()
{
	assert(linkedActorSystem);
	linkedActorSystem->RemoveActor(this);
}

void Actor::AddChild(Actor* child)
{
	assert(child);
	child->parent = this;
	child->isRoot = false;
	children.push_back(child);
}

void ActorSystem::Serialise(std::ostream& os)
{
	os << name << "\n"; //Use actorsystem name to create again from ActorSystemFactory on Deserialise
	os << actors.size() << "\n"; //Write out num of actors to load the same amount on Deserialise

	//Writing these three out is more about instancing prefabs off of ActorSystem so that you're not 
	//defining these again by hand in new cpp files.
	os << modelName.c_str() << "\n";
	os << textureName.c_str() << "\n";
	os << shaderName.c_str() << "\n";

	for(Actor* actor : actors)
	{
		Serialiser::Serialise(actor->GetSaveProps(), os);
	}
}

void ActorSystem::SerialiseAsTemplate(std::ostream& os)
{
	os << name << "\n";
	os << modelName.c_str() << "\n";
	os << textureName.c_str() << "\n";
	os << shaderName.c_str() << "\n";
}

void ActorSystem::Deserialise(std::istream& is)
{
	char name[512];
	is.getline(name, 512);
	is.getline(name, 512);
	modelName.assign(name);

	is.getline(name, 512);
	textureName.assign(name);

	is.getline(name, 512);
	shaderName.assign(name);

	for(Actor* actor : actors)
	{
		Serialiser::Deserialise(actor->GetSaveProps(), is);
	}
}

void ActorSystem::DeserialiseAsTemplate(std::istream& is)
{
	is >> name;
	is >> modelName;
	is >> textureName;
	is >> shaderName;
}

void ActorSystem::Tick(float deltaTime)
{

}

void ActorSystem::Start()
{
}

void ActorSystem::SpawnActors(int numToSpawn)
{
	Init<Actor>(numToSpawn);
}

Actor* ActorSystem::SpawnActor(Transform transform)
{
	return AddActor<Actor>(transform);
}

Buffer* ActorSystem::GetVertexBuffer()
{
	if (pso.vertexBuffer)
	{
		return pso.vertexBuffer;
	}

	return nullptr;
}

Buffer* ActorSystem::GetInstanceBuffer()
{
	if (pso.instanceBuffer)
	{
		return pso.instanceBuffer;
	}

	return nullptr;
}

void ActorSystem::RemoveActor(Actor* actor)
{
	//TODO: this looks bad (but honestly, I don't think the performance is too bad).
	//Either way look into shared_ptr and how it can be set into the actors vector
	for(int i = 0; i < actors.size(); i++)
	{
		if (&actor[i] == actor)
		{
			actors.erase(actors.begin() + i);
		}
	}
}

void ActorSystem::RemoveActor(int index)
{
	assert(index < actors.size());
	actors.erase(actors.begin() + index);
}

Actor* ActorSystem::GetActor(unsigned int index)
{
	assert(index < actors.size());
	return actors[index];
}

Sampler* ActorSystem::GetSamplerState()
{
	return pso.samplerState;
}

RasterizerState* ActorSystem::GetRasterizerState()
{
	if (pso.rastState)
	{
		return pso.rastState;
	}

	return nullptr;
}

ShaderResourceView* ActorSystem::GetShaderView()
{
	if (pso.srv)
	{
		return pso.srv;
	}

	return nullptr;
}

Texture* ActorSystem::GetTexture()
{
	if (pso.texture)
	{
		return pso.texture;
	}

	return nullptr;
}

void ActorSystem::SetTexture(Texture* texture)
{
	pso.texture = texture;
}

void ActorSystem::CreateStructuredBuffer()
{
	std::vector<XMMATRIX> actorModelMatrices;
	actorModelMatrices.reserve(actors.size());
	for (int i = 0; i < actors.size(); i++)
	{
		if (i < actors.size())
		{
			actorModelMatrices.push_back(actors[i]->GetTransformationMatrix());
		}
		else
		{
			actorModelMatrices.push_back(XMMatrixIdentity());
		}
	}

	if (pso.instancedDataStructuredBuffer)
	{
		//TODO: gotta fix this up. Maybe wrap this in a ComPtr.
		//instancedDataStructuredBuffer->Release();
	}

	pso.instancedDataStructuredBuffer->data = gRenderSystem.CreateStructuredBuffer(sizeof(InstanceData) * actors.size(), sizeof(InstanceData), actorModelMatrices.data());

	D3D11_SHADER_RESOURCE_VIEW_DESC sbDesc = {};
	sbDesc.Format = DXGI_FORMAT_UNKNOWN;
	sbDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	sbDesc.BufferEx.NumElements = actors.size();
	HR(gRenderSystem.device->CreateShaderResourceView(pso.instancedDataStructuredBuffer->data, &sbDesc, &pso.instancedDataSrv->data));
}

//For when texture file is changed in-editor.
void ActorSystem::RecreateTexture()
{
	pso.texture->data->Release();
	pso.srv->data->Release();
	gRenderSystem.CreateTexture(this);
}

void ActorSystem::RecreateModel()
{
	pso.vertexBuffer->data->Release();

	modelData.DeleteAll();

	std::string filename = "Models/";
	filename += modelName;
	if (FBXImporter::Import(filename.c_str(), modelData, this))
	{
		gRenderSystem.CreateVertexBuffer(modelData.GetByteWidth(), modelData.verts.data(), this);

		size_t stride = sizeof(Vertex);
		BoundingOrientedBox::CreateFromPoints(boundingBox, modelData.verts.size(), &modelData.verts[0].pos, stride);
		BoundingSphere::CreateFromPoints(boundingSphere, modelData.verts.size(), &modelData.verts[0].pos, stride);
	}
}

void ActorSystem::Cleanup()
{
	actors.clear();
}

void ActorSystem::SetVertexBuffer(Buffer* vertexBuffer)
{
	pso.vertexBuffer = vertexBuffer;
}

void ActorSystem::SetInstanceBuffer(Buffer* instanceBuffer)
{
	pso.instanceBuffer = instanceBuffer;
}

void ActorSystem::SetSamplerState(Sampler* sampler)
{
	pso.samplerState = sampler;
}

void ActorSystem::SetRasterizerState(RasterizerState* rasterizerState)
{
	pso.rastState = rasterizerState;
}

void ActorSystem::SetShaderView(ShaderResourceView* shaderView)
{
	pso.srv = shaderView;
}
