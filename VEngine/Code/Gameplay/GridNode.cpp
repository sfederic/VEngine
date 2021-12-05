#include "GridNode.h"
#include "Gameplay/GameUtils.h"
#include "Actors/Game/BattleGrid.h"
#include "Components/InstanceMeshComponent.h"

void GridNode::Hide()
{
	auto grid = GameUtils::GetBattleGrid();
	auto& meshInstanceData = grid->nodeMesh->instanceData[instancedMeshIndex];

	//Mul by empty scale matrix to make the node invisible in scene
	XMMATRIX emptyScaleMatrix = XMMatrixScaling(0.f, 0.f, 0.f);
	meshInstanceData.world *= emptyScaleMatrix;
}

void GridNode::Show()
{	
	auto grid = GameUtils::GetBattleGrid();
	auto& meshInstanceData = grid->nodeMesh->instanceData[instancedMeshIndex];

	//Set uniform identity scale
	meshInstanceData.world.r[0].m128_f32[0] = 0.9f;
	meshInstanceData.world.r[1].m128_f32[1] = 0.9f;
	meshInstanceData.world.r[2].m128_f32[2] = 0.9f;
}
