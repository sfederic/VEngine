#include "vpch.h"
#include "GridNode.h"
#include "Gameplay/GameUtils.h"
#include "Actors/Game/Grid.h"
#include "Components/InstanceMeshComponent.h"
#include "Physics/Raycast.h"
#include "VMath.h"

void GridNode::Hide()
{
	active = false;

	auto grid = GameUtils::GetGrid();
	auto& meshInstanceData = grid->nodeMesh->instanceData[instancedMeshIndex];

	meshInstanceData.world.r[0].m128_f32[0] = 0.f;
	meshInstanceData.world.r[1].m128_f32[1] = 0.f;
	meshInstanceData.world.r[2].m128_f32[2] = 0.f;
}

void GridNode::Show()
{	
	active = true;

	auto grid = GameUtils::GetGrid();
	auto& meshInstanceData = grid->nodeMesh->instanceData[instancedMeshIndex];

	meshInstanceData.world.r[0].m128_f32[0] = 0.9f;
	meshInstanceData.world.r[1].m128_f32[1] = 0.9f;
	meshInstanceData.world.r[2].m128_f32[2] = 0.9f;
}

void GridNode::DisplayHide()
{
	auto grid = GameUtils::GetGrid();
	auto& meshInstanceData = grid->nodeMesh->instanceData[instancedMeshIndex];

	meshInstanceData.world.r[0].m128_f32[0] = 0.f;
	meshInstanceData.world.r[1].m128_f32[1] = 0.f;
	meshInstanceData.world.r[2].m128_f32[2] = 0.f;
}

void GridNode::DisplayShow()
{
	auto grid = GameUtils::GetGrid();
	auto& meshInstanceData = grid->nodeMesh->instanceData[instancedMeshIndex];

	meshInstanceData.world.r[0].m128_f32[0] = 0.9f;
	meshInstanceData.world.r[1].m128_f32[1] = 0.9f;
	meshInstanceData.world.r[2].m128_f32[2] = 0.9f;
}

void GridNode::RecalcNodeHeight(Ray& ray)
{
	XMVECTOR origin = XMVectorSet((float)xIndex, 20.f, (float)yIndex, 1.f);
	if (Raycast(ray, origin, -VMath::XMVectorUp(), 40.f))
	{
		auto grid = GameUtils::GetGrid();
		auto& meshInstanceData = grid->nodeMesh->instanceData[instancedMeshIndex];

		XMFLOAT3 hitPos = ray.hitPos;
		hitPos.y += 0.1f;
		XMVECTOR hitPosVector = XMLoadFloat3(&hitPos);
		hitPosVector.m128_f32[3] = 1.0f;

		//set the y-pos for the node
		worldPosition.y = hitPos.y + 0.4f;

		meshInstanceData.world.r[3] = hitPosVector;
	}
}

void GridNode::SetColour(XMFLOAT4 newColour)
{
	auto grid = GameUtils::GetGrid();
	auto& meshInstanceData = grid->nodeMesh->instanceData[instancedMeshIndex];

	meshInstanceData.colour = newColour;
}
