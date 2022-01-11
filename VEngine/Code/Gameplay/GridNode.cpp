#include "GridNode.h"
#include "Gameplay/GameUtils.h"
#include "Actors/Game/Grid.h"
#include "Components/InstanceMeshComponent.h"

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
