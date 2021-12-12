#define NOMINMAX

#include "Unit.h"
#include <algorithm>
#include "Gameplay/GridNode.h"
#include "Gameplay/GameUtils.h"
#include "Grid.h"
#include "Components/MeshComponent.h"
#include "VMath.h"
#include "Player.h"
#include "Gameplay/BattleSystem.h"
#include "TimerSystem.h"
#include "Log.h"
#include "UI/GuardWidget.h"

Unit::Unit()
{
}

void Unit::Start()
{
	__super::Start();

	nextMovePos = GetPositionVector();
}

void Unit::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	if (isUnitTurn && !attackWindingUp)
	{
		if (XMVector4Equal(nextMovePos, GetPositionVector()))
		{
			if (movementPathNodeIndex < pathNodes.size())
			{
				nextMovePos = XMLoadFloat3(&pathNodes[movementPathNodeIndex]->worldPosition);

				xIndex = pathNodes[movementPathNodeIndex]->xIndex;
				yIndex = pathNodes[movementPathNodeIndex]->yIndex;

				movementPathNodeIndex++;
			}
			else if (movementPathNodeIndex >= pathNodes.size())
			{
				movementPathNodeIndex = 0;
				pathNodes.clear();

				GetCurrentNode()->Hide();

				if (Attack())
				{
					//deal with attack wind up
					attackWindingUp = true;

					auto player = GameUtils::GetPlayer();
					player->guardWidget->AddToViewport();

					player->nextCameraFOV = 30.f;
					GameUtils::SetActiveCameraTarget(this);

					timerSystem.SetTimer(2.f, std::bind(&Unit::WindUpAttack, this));
				}
				else
				{
					//end turn
					isUnitTurn = false;
					battleSystem.MoveToNextTurn();
				}
			}
		}
	}

	SetPosition(VMath::VectorConstantLerp(GetPositionVector(), nextMovePos, deltaTime, moveSpeed));
}

Properties Unit::GetProps()
{
	auto props = __super::GetProps();
	props.Add("Move Points", &movementPoints);
	props.Add("NextMove", &nextMovePos);
	props.Add("Move Speed", &moveSpeed);
	props.Add("Attack Points", &attackPoints);
	return props;
}

void Unit::InflictDamage(int damage)
{
	if (health <= damage && isDestructible)
	{
		battleSystem.RemoveUnit(this);
	}

	__super::InflictDamage(damage);
}

void Unit::MoveToNode(GridNode* destinationNode)
{
	auto grid = GameUtils::GetGrid();
	GridNode* startingNode = grid->GetNode(xIndex, yIndex);

	std::vector<GridNode*> nodes;
	std::vector<GridNode*> closedNodes;

	grid->GetNeighbouringNodes(startingNode, nodes);

	for (int moveIndex = 0; moveIndex < movementPoints; moveIndex++)
	{
		for (int previewIndex = 0; previewIndex < nodes.size(); previewIndex++)
		{
			grid->GetNeighbouringNodes(nodes[previewIndex], closedNodes);
		}

		nodes.insert(nodes.end(), closedNodes.begin(), closedNodes.end());

		closedNodes.clear();
	}

	for (GridNode* node : nodes)
	{
		//reset the closes state
		node->closed = false;

		movementPathNodes.push_back(node);
	}

	//Assign all costs
	for (auto node : movementPathNodes)
	{
		//TODO: come back here and see if getting the distances with just the int x/yIndex values is enough.
		//I'm guessing the Vector distance stuff here was for node height originally.
		XMVECTOR startPos = XMLoadFloat3(&startingNode->worldPosition);
		XMVECTOR endPos = XMLoadFloat3(&destinationNode->worldPosition);
		XMVECTOR currentPos = XMLoadFloat3(&node->worldPosition);

		node->gCost = XMVector3Length(startPos - currentPos).m128_f32[0];
		node->hCost = XMVector3Length(endPos - currentPos).m128_f32[0];
	}


	GridNode* nextNode = nullptr;

	int highestHCostIndex = 0;
	int lowestHCostIndex = 0;

	float lowestHCost = std::numeric_limits<float>::max();
	for (int i = 0; i < movementPathNodes.size(); i++)
	{
		if (movementPathNodes[i]->hCost < lowestHCost)
		{
			lowestHCost = movementPathNodes[i]->hCost;
			lowestHCostIndex = i;
			nextNode = movementPathNodes[i];
		}
	}

	//This needs to be here if you need Units moving exactly to a point, but most times
	// the unit is going to be moving to a surrounding node if there is a target, eg. the unit
	// isn't going to move onto the node of the player, instead to a neighbouring node
	//if (nextNode != nullptr)
	//{
	//	pathNodes.push_back(nextNode);
	//}

	if (nextNode)
	{
		while (nextNode != startingNode)
		{
			if (nextNode->parentNode)
			{
				nextNode = nextNode->parentNode;
				pathNodes.push_back(nextNode);
			}
		}
	}

	std::reverse(pathNodes.begin(), pathNodes.end());

	movementPathNodes.clear();
}

void Unit::MoveToNode(int x, int y)
{
	auto grid = GameUtils::GetGrid();
	GridNode* destinationNode = grid->GetNode(x, y);

	MoveToNode(destinationNode);
}

void Unit::StartTurn()
{
	isUnitTurn = true;

	auto node = GetCurrentNode();
	node->active = true;

	auto player = GameUtils::GetPlayer();
	MoveToNode(player->xIndex, player->yIndex);
}

bool Unit::Attack()
{
	auto standingNode = GetCurrentNode();
	auto grid = GameUtils::GetGrid();

	std::vector<GridNode*> attackNodes;
	grid->GetNeighbouringNodes(standingNode, attackNodes);

	auto player = GameUtils::GetPlayer();
	auto targetNode = player->GetCurrentNode();

	for (auto node : attackNodes)
	{
		if (node->Equals(targetNode))
		{
			//Attack can hit
			return true;
		}
	}

	//Attack can't hit
	return false;
}

void Unit::WindUpAttack()
{
	auto player = GameUtils::GetPlayer();
	player->InflictDamage(attackPoints);

	player->guardWidget->RemoveFromViewport();

	player->nextCameraFOV = 60.f;
	GameUtils::SetActiveCameraTarget(player);

	Log("%s attacked %s", this->name.c_str(), player->name.c_str());
}
