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
#include "Timer.h"
#include "Log.h"
#include "UI/GuardWidget.h"
#include "Gameplay/GameInstance.h"

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
					player->ableToGuard = true;
					player->nextCameraFOV = 30.f;

					GameUtils::SetActiveCameraTarget(this);

					Timer::SetTimer(2.f, std::bind(&Unit::WindUpAttack, this));
				}
				else
				{
					EndTurn();
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
	props.AddProp(deathText);
	return props;
}

void Unit::InflictDamage(int damage)
{
	if (health <= damage && isDestructible)
	{
		GameInstance::AddGuilt();

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
		//Assigning g/hCosts here based on world positions to account for node heights
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

	GetCurrentNode()->Show();

	auto player = GameUtils::GetPlayer();
	MoveToNode(player->xIndex, player->yIndex);
}

void Unit::EndTurn()
{
	isUnitTurn = false;
	battleSystem.MoveToNextTurn();
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

	player->ableToGuard = false;
	player->guardWidget->RemoveFromViewport();
	player->guardWidget->guardSuccessful = false;

	GameUtils::PlayAudioOneShot("sword_hit.wav");

	if (player->guarding)
	{
		GameUtils::PlayAudioOneShot("shield_hit.wav");
	}

	player->InflictDamage(attackPoints);

	player->nextCameraFOV = 60.f;
	GameUtils::SetActiveCameraTarget(player);

	Log("%s attacked %s", this->name.c_str(), player->name.c_str());
	
	attackWindingUp = false;

	EndTurn();
}
