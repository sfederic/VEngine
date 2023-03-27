#include "vpch.h"
#include "GridNode.h"
#include "Actors/Game/Grid.h"
#include "Components/InstanceMeshComponent.h"
#include "Physics/Raycast.h"
#include "Core/VMath.h"

void GridNode::Hide()
{
	active = false;

	auto grid = Grid::system.GetFirstActor();
	auto& meshInstanceData = grid->nodeMesh->GetInstanceData()[instancedMeshIndex];

	meshInstanceData.world.r[0].m128_f32[0] = 0.f;
	meshInstanceData.world.r[1].m128_f32[1] = 0.f;
	meshInstanceData.world.r[2].m128_f32[2] = 0.f;
}

void GridNode::Show()
{	
	active = true;

	auto grid = Grid::system.GetFirstActor();
	auto& meshInstanceData = grid->nodeMesh->GetInstanceData()[instancedMeshIndex];

	meshInstanceData.world.r[0].m128_f32[0] = 0.9f;
	meshInstanceData.world.r[1].m128_f32[1] = 0.9f;
	meshInstanceData.world.r[2].m128_f32[2] = 0.9f;
}

void GridNode::DisplayHide()
{
	auto grid = Grid::system.GetFirstActor();
	auto& meshInstanceData = grid->nodeMesh->GetInstanceData()[instancedMeshIndex];

	meshInstanceData.world.r[0].m128_f32[0] = 0.f;
	meshInstanceData.world.r[1].m128_f32[1] = 0.f;
	meshInstanceData.world.r[2].m128_f32[2] = 0.f;
}

void GridNode::DisplayShow()
{
	auto grid = Grid::system.GetFirstActor();
	auto& meshInstanceData = grid->nodeMesh->GetInstanceData()[instancedMeshIndex];

	meshInstanceData.world.r[0].m128_f32[0] = 0.9f;
	meshInstanceData.world.r[1].m128_f32[1] = 0.9f;
	meshInstanceData.world.r[2].m128_f32[2] = 0.9f;
}

void GridNode::RecalcNodeHeight(HitResult& hitResult)
{
	XMVECTOR origin = XMVectorSet((float)xIndex, 20.f, (float)yIndex, 1.f);
	if (Raycast(hitResult, origin, -VMath::GlobalUpVector(), 40.f))
	{
		auto grid = Grid::system.GetFirstActor();
		auto& meshInstanceData = grid->nodeMesh->GetInstanceData()[instancedMeshIndex];

		XMFLOAT3 hitPos = hitResult.hitPos;
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
	auto grid = Grid::system.GetFirstActor();
	auto& meshInstanceData = grid->nodeMesh->GetInstanceData()[instancedMeshIndex];

	meshInstanceData.colour = newColour;
}
