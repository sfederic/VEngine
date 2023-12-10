#include "vpch.h"
#include "Grid.h"
#include "Components/InstanceMeshComponent.h"
#include "Render/RenderUtils.h"
#include "Render/Material.h"
#include "Render/BlendStates.h"
#include "Physics/Raycast.h"
#include "Core/VMath.h"
#include "GridActor.h"
#include "Unit.h"
#include "Actors/Game/Player.h"

Grid::Grid()
{
	nodeMesh = InstanceMeshComponent::system.Add("NodeMesh",
		this, InstanceMeshComponent(0, "node.vmesh", "test.png", ShaderItems::Instance));
	rootComponent = nodeMesh;
}

//Grid::Awake() can also be used to reset all node world positions during gameplay
void Grid::Awake()
{
	__super::Awake();

	HitResult hit(this);
	RecalcAllNodes(hit);
}

void Grid::Create()
{
	__super::Create();

	nodeMesh->SetBlendState(BlendStates::Default);
}

void Grid::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	switch (lerpSetting)
	{
	case LerpSetting::LerpIn:
		LerpInNodes(deltaTime);
		break;

	case LerpSetting::LerpOut:
		LerpOutNodes(deltaTime);
		break;
	}
}

Properties Grid::GetProps()
{
	auto props = __super::GetProps();
	props.title = "BattleNode";
	props.Add("Size X", &sizeX);
	props.Add("Size Y", &sizeY);
	return props;
}

GridNode* Grid::GetNode(int x, int y)
{
	assert(x < rows.size());
	assert(y < rows[x].columns.size());
	return &rows[x].columns[y];
}

GridNode* Grid::GetNodeAllowNull(int x, int y)
{
	if (x < 0) return nullptr;
	if (y < 0) return nullptr;
	if (x >= sizeX) return nullptr;
	if (y >= sizeY) return nullptr;
	return &rows[x].columns[y];
}

std::vector<GridNode*> Grid::GetAllNodes()
{
	std::vector<GridNode*> outNodes;

	for (int x = 0; x < sizeX; x++)
	{
		for (int y = 0; y < sizeX; y++)
		{
			outNodes.push_back(GetNode(x, y));
		}
	}

	return outNodes;
}

void Grid::RecalcAllNodes(HitResult& hit)
{
	nodeMesh->ReleaseBuffers();

	//Set the mesh count as 1 and empty the data just to put dummy data into the buffers
	nodeMesh->GetInstanceData().clear();
	nodeMesh->GetInstanceData().push_back(InstanceData());

	int meshInstanceCount = sizeX * sizeY;
	nodeMesh->SetInstanceCount(meshInstanceCount);

	//Re-setup shader buffers
	nodeMesh->structuredBuffer = RenderUtils::CreateStructuredBuffer(sizeof(InstanceData) * meshInstanceCount,
		sizeof(InstanceData), nodeMesh->GetInstanceData().data());

	nodeMesh->srv = RenderUtils::CreateSRVForMeshInstance(nodeMesh->structuredBuffer, meshInstanceCount);

	const XMMATRIX rootWorldMatrix = rootComponent->GetWorldMatrix();
	XMVECTOR rayOrigin = XMVectorZero();

	nodeMesh->GetInstanceData().clear();

	//Ignore player and units
	hit.ignoreLayer = CollisionLayers::Editor;
	hit.actorsToIgnore.push_back((Actor*)Player::system.GetFirstActor());
	auto gridActors = World::GetAllActorsOfTypeAsActor<GridActor>();
	for (auto gridActor : gridActors)
	{
		auto castActor = (GridActor*)gridActor;
		if (castActor)
		{
			if (castActor->isGridObstacle)
			{
				continue;
			}
		}

		hit.actorsToIgnore.push_back(gridActor);
	}

	auto meshesToIgnore = World::GetAllComponentsOfType<MeshComponent>();
	for (auto mesh : meshesToIgnore)
	{
		if (mesh->ignoreGridRaycasts)
		{
			hit.componentsToIgnore.push_back(mesh);
		}
	}

	rows.clear();

	for (int x = 0; x < sizeX; x++)
	{
		//create grid row
		rows.push_back(GridRow());

		for (int y = 0; y < sizeY; y++)
		{
			rayOrigin = XMVectorSet(x, 20.f, y, 1.f);

			//Set instance model matrix
			InstanceData instanceData = {};
			instanceData.world = rootWorldMatrix;

			//create grid node in row
			GridNode node = GridNode(x, y, nodeMesh->GetInstanceData().size());

			instanceData.colour = GridNode::normalColour;

			hit.hitActor = nullptr;
			hit.hitActors.clear();
			hit.hitComponent = nullptr;
			hit.hitComponents.clear();

			//raycast against the world to set node position
			if (Raycast(hit, rayOrigin, -VMath::GlobalUpVector(), 40.0f))
			{
				//Position the node at the raycast's hitpos
				XMFLOAT3 hitPos = hit.hitPos;
				hitPos.y += 0.1f;
				XMVECTOR hitPosVector = XMLoadFloat3(&hitPos);
				hitPosVector.m128_f32[3] = 1.0f;

				//Set the y-pos for the node. This is slightly higher so that GridActors are moving to the 
				//correct positions in world on their next moves as opposed to the node's actual position.
				node.worldPosition.y = hitPos.y + 0.4f;

				//Make node rotation match the hit normal.
				instanceData.world = VMath::MakeRotationFromYAxis(hit.GetNormalV());
				instanceData.world.r[3] = hitPosVector;

				//Scale the node down to nothing so that nodes are 'hidden' on world load
				instanceData.world.r[0].m128_f32[0] = 0.f;
				instanceData.world.r[1].m128_f32[1] = 0.f;
				instanceData.world.r[2].m128_f32[2] = 0.f;

				node.active = true;

				if (hit.hitActor)
				{
					auto meshes = hit.hitActor->GetComponents<MeshComponent>();
					for (auto mesh : meshes)
					{
						if (mesh->gridObstacle)
						{
							node.active = false;
						}
					}
				}
			}
			else
			{
				XMMATRIX emptyScaleMatrix = XMMatrixScaling(0.f, 0.f, 0.f);
				instanceData.world *= emptyScaleMatrix;

				node.active = false;
			}

			nodeMesh->GetInstanceData().push_back(instanceData);

			//Add node to column
			rows[x].Add(node);
		}
	}
}

void Grid::RecalcNodesToIgnoreLinkedGridActor(GridActor* gridActorToIgnore)
{
	HitResult hit(this);
	hit.actorsToIgnore.push_back(gridActorToIgnore);
	for (auto mesh : gridActorToIgnore->GetComponents<MeshComponent>())
	{
		hit.componentsToIgnore.push_back(mesh);
	}
	RecalcAllNodes(hit);
}

GridNode* Grid::GetNodeLimit(int x, int y)
{
	if (x < 0) x = 0;
	if (y < 0) y = 0;
	if (x >= sizeX) x = sizeX - 1;
	if (y >= sizeY) y = sizeY - 1;
	return &rows[x].columns[y];
}

void Grid::GetNeighbouringNodes(GridNode* centerNode, std::vector<GridNode*>& outNodes)
{
	int currentX = centerNode->xIndex;
	int currentY = centerNode->yIndex;

	//+X
	if (currentX < (sizeX - 1))
	{
		GridNode& node = rows[currentX + 1].columns[currentY];
		if (!node.closed && node.active)
		{
			if (node.worldPosition.y < (centerNode->worldPosition.y + Grid::maxHeightMove))
			{
				node.closed = true;
				node.parentNode = centerNode;
				outNodes.push_back(&node);
			}
		}
	}

	//-X
	if (currentX > 0)
	{
		GridNode& node = rows[currentX - 1].columns[currentY];
		if (!node.closed && node.active)
		{
			if (node.worldPosition.y < (centerNode->worldPosition.y + Grid::maxHeightMove))
			{
				node.closed = true;
				node.parentNode = centerNode;
				outNodes.push_back(&node);
			}
		}
	}

	//+Y
	if (currentY < (sizeY - 1))
	{
		GridNode& node = rows[currentX].columns[currentY + 1];
		if (!node.closed && node.active)
		{
			if (node.worldPosition.y < (centerNode->worldPosition.y + Grid::maxHeightMove))
			{
				node.closed = true;
				node.parentNode = centerNode;
				outNodes.push_back(&node);
			}
		}
	}

	//-Y
	if (currentY > 0)
	{
		GridNode& node = rows[currentX].columns[currentY - 1];
		if (!node.closed && node.active)
		{
			if (node.worldPosition.y < (centerNode->worldPosition.y + Grid::maxHeightMove))
			{
				node.closed = true;
				node.parentNode = centerNode;
				outNodes.push_back(&node);
			}
		}
	}
}

void Grid::GetNeighbouringNodesForceful(GridNode* centerNode, std::vector<GridNode*>& outNodes)
{
	int currentX = centerNode->xIndex;
	int currentY = centerNode->yIndex;

	//+X
	if (currentX < (sizeX - 1))
	{
		GridNode& node = rows[currentX + 1].columns[currentY];
		if (node.active)
		{
			outNodes.push_back(&node);
		}
	}

	//-X
	if (currentX > 0)
	{
		GridNode& node = rows[currentX - 1].columns[currentY];
		if (node.active)
		{
			outNodes.push_back(&node);
		}
	}

	//+Y
	if (currentY < (sizeY - 1))
	{
		GridNode& node = rows[currentX].columns[currentY + 1];
		if (node.active)
		{
			outNodes.push_back(&node);
		}
	}

	//-Y
	if (currentY > 0)
	{
		GridNode& node = rows[currentX].columns[currentY - 1];
		if (node.active)
		{
			outNodes.push_back(&node);
		}
	}
}

std::vector<GridNode*> Grid::GetNeighbouringActiveAndInactiveNodesForceful(GridNode* centerNode)
{
	std::vector<GridNode*> outNodes;

	int currentX = centerNode->xIndex;
	int currentY = centerNode->yIndex;

	//+X
	if (currentX < (sizeX - 1))
	{
		GridNode& node = rows[currentX + 1].columns[currentY];
		outNodes.push_back(&node);
	}

	//-X
	if (currentX > 0)
	{
		GridNode& node = rows[currentX - 1].columns[currentY];
		outNodes.push_back(&node);
	}

	//+Y
	if (currentY < (sizeY - 1))
	{
		GridNode& node = rows[currentX].columns[currentY + 1];
		outNodes.push_back(&node);
	}

	//-Y
	if (currentY > 0)
	{
		GridNode& node = rows[currentX].columns[currentY - 1];
		outNodes.push_back(&node);
	}

	return outNodes;
}

Unit* Grid::GetUnitAtNode(GridNode* node)
{
	for (auto& unit : Unit::system.GetActors())
	{
		auto unitCurrentNode = unit->GetCurrentNode();
		if (node->Equals(unitCurrentNode))
		{
			return unit.get();
		}
	}

	return nullptr;
}

Unit* Grid::GetUnitAtNodeIndex(int xIndex, int yIndex)
{
	auto node = GetNodeAllowNull(xIndex, yIndex);
	if (node)
	{
		return GetUnitAtNode(node);
	}

	return nullptr;
}

std::vector<Unit*> Grid::GetAllUnitsFromNodes(std::vector<GridNode*>& nodes)
{
	std::vector<Unit*> units;
	for (auto node : nodes)
	{
		auto unit = GetUnitAtNode(node);
		if (unit)
		{
			units.push_back(unit);
		}
	}
	return units;
}

void Grid::ResetAllNodes()
{
	for (auto& row : rows)
	{
		for (auto& node : row.columns)
		{
			node.ResetValues();
		}
	}
}

void Grid::LerpInNodes(float deltaTime)
{
	constexpr float lerpSpeed = 4.5f;
	constexpr float lerpMin = 0.01f;
	constexpr float tolerance = 0.01f;

	currentLerpValue = std::lerp(currentLerpValue, lerpMin, deltaTime * lerpSpeed);
	if (currentLerpValue < lerpMin + tolerance)
	{
		nodeMesh->SetVisibility(false);
		return;
	}

	SetAllNodesToCurrentLerpValue();
}

void Grid::LerpOutNodes(float deltaTime)
{
	constexpr float lerpSpeed = 4.5f;
	constexpr float lerpMax = 0.9f;
	constexpr float tolerance = 0.01f;

	currentLerpValue = std::lerp(currentLerpValue, lerpMax, deltaTime * lerpSpeed);
	if (currentLerpValue > lerpMax - tolerance)
	{
		return;
	}

	nodeMesh->SetVisibility(true);
	SetAllNodesToCurrentLerpValue();
}

void Grid::DisplayHideAllNodes()
{
	lerpSetting = LerpSetting::LerpIn;

	for (auto& row : rows)
	{
		for (auto& node : row.columns)
		{
			node.DisplayHide();
		}
	}
}

void Grid::DisplayShowAllNodes()
{
	lerpSetting = LerpSetting::LerpOut;

	for (auto& row : rows)
	{
		for (auto& node : row.columns)
		{
			node.DisplayShow();
		}
	}
}

std::vector<InstanceData>& Grid::GetNodeMeshInstanceData()
{
	return nodeMesh->GetInstanceData();
}

void Grid::SetGridSize(int x, int y)
{
	sizeX = x;
	sizeY = y;
}

void Grid::SetAllNodesToCurrentLerpValue()
{
	auto& instanceData = nodeMesh->GetInstanceData();

	for (auto& row : rows)
	{
		for (auto& node : row.columns)
		{
			if (!node.active || node.preview)
			{
				continue;
			}

			XMMATRIX& worldMatrix = instanceData.at(node.instancedMeshIndex).world;
			worldMatrix.r[0].m128_f32[0] = currentLerpValue;
			worldMatrix.r[1].m128_f32[1] = currentLerpValue;
			worldMatrix.r[2].m128_f32[2] = currentLerpValue;
		}
	}
}
