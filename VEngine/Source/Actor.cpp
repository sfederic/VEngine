#include "Actor.h"
#include "RenderSystem.h"
#include "Array.h"
#include "Debug.h"
#include "Obj.h"
#include "MathHelpers.h"
#include "IRenderSystem.h"
#include "IBuffer.h"
#include "IRasterizerState.h"
#include "IShaderView.h"
#include "ISampler.h"
#include "IShaderView.h"
#include "ITexture.h"

//CONSTRUCTORS
Actor::Actor()
{
	transform = XMMatrixIdentity();
}

//POSITION FUNCTIONS
XMVECTOR Actor::GetPositionVector()
{
	return transform.r[3];
}

XMFLOAT3 Actor::GetPositionFloat3()
{
	XMVECTOR pos = transform.r[3];
	return XMFLOAT3(pos.m128_f32[0], pos.m128_f32[1], pos.m128_f32[2]);
}

void Actor::SetPosition(XMVECTOR v)
{
	transform.r[3] = v;
}

void Actor::SetPosition(float x, float y, float z)
{
	XMVECTOR v = XMVectorSet(x, y, z, 1.0f);
	transform.r[3] = v;
}

void Actor::SetPosition(XMFLOAT3 pos)
{
	transform.r[3] = XMVectorSet(pos.x, pos.y, pos.z, 1.0f);
}

//ROTATION FUNCTIONS
void Actor::SetRotation(XMVECTOR axis, float angle)
{
	XMMATRIX m = XMMatrixRotationAxis(axis, angle);
	transform = XMMatrixMultiply(m, transform); //Just keep in mind Matrix mul order
}

void Actor::SetRotation(float roll, float pitch, float yaw)
{
	transform *= XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
}

void Actor::SetRotation(XMFLOAT3 rollPitchYaw)
{
	transform *= XMMatrixRotationRollPitchYaw(rollPitchYaw.x, rollPitchYaw.y, rollPitchYaw.z);
}

XMMATRIX Actor::GetRotation()
{
	return transform;
}

XMFLOAT3 Actor::GetRollPitchYaw()
{
	XMVECTOR vec = RollPitchYawFromMatrix(transform);
	XMFLOAT3 rot(vec.m128_f32[0], vec.m128_f32[1], vec.m128_f32[2]);
	return rot;
}

//SCALE FUNCTIONS
XMFLOAT3 Actor::GetScale()
{
	return XMFLOAT3(transform.r[0].m128_f32[0], transform.r[1].m128_f32[1], transform.r[2].m128_f32[2]);
}

void Actor::AddScale(float scale)
{

}

void Actor::AddScale(float x, float y, float z)
{

}

void Actor::AddScale(XMFLOAT3 scale)
{
	
}

void Actor::SetScale(float x, float y, float z)
{
	transform *= XMMatrixScaling(x, y, z);
}

void Actor::SetScale(XMVECTOR scale)
{
	transform *= XMMatrixScalingFromVector(scale);
}

void Actor::SetScale(XMFLOAT3 scale)
{
	XMVECTOR scaleVec = XMLoadFloat3(&scale);
	transform *= XMMatrixScalingFromVector(scaleVec);
}

//VECTOR FUNCTIONS
XMVECTOR Actor::GetForwardVector()
{
	return transform.r[2];
}

XMVECTOR Actor::GetRightVector()
{
	return transform.r[0];
}

XMVECTOR Actor::GetUpVector()
{
	return transform.r[1];
}

void Actor::Move(float d, XMVECTOR direction)
{
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR loc = GetPositionVector();
	loc = XMVectorMultiplyAdd(s, direction, loc);
	SetPosition(loc);
}

//ACTOR SYSTEM
ActorSystem::ActorSystem()
{
	pso.Create();
}

void* ActorSystem::GetVertexBuffer()
{
	return pso.vertexBuffer->Get();
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


void* ActorSystem::GetSamplerState()
{
	return pso.samplerState->Get();
}

void* ActorSystem::GetRasterizerState()
{
	if (pso.rastState)
	{
		return pso.rastState->Get();
	}

	return nullptr;
}

void* ActorSystem::GetShaderView()
{
	return pso.srv->Get();
}

void* ActorSystem::GetTexture()
{
	return nullptr;
}

void ActorSystem::SetTexture(ITexture* texture)
{
	pso.texture = texture;
}

void ActorSystem::SetVertexBuffer(IBuffer* vertexBuffer)
{
	pso.vertexBuffer = vertexBuffer;
}

void ActorSystem::SetSamplerState(ISampler* sampler)
{
	pso.samplerState = sampler;
}

void ActorSystem::SetRasterizerState(IRasterizerState* rasterizerState)
{
	pso.rastState = rasterizerState;
}

void ActorSystem::SetShaderView(IShaderView* shaderView)
{
	pso.srv = shaderView;
}

//PIPELINE VIEW
void PipelineView::Create()
{
	vertexBuffer = new D3D11Buffer();
	indexBuffer = new D3D11Buffer();
	samplerState = new D3D11Sampler();
	rastState = new D3D11RasterizerState();
	texture = new D3D11Texture();
	srv = new D3D11ShaderView();
}
