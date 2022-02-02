#define NOMINMAX //bruh...

#include "Unit.h"
#include <algorithm>
#include "Gameplay/GridNode.h"
#include "Gameplay/GameUtils.h"
#include "Grid.h"
#include "Components/MeshComponent.h"
#include "Components/MemoryComponent.h"
#include "VMath.h"
#include "Player.h"
#include "Gameplay/BattleSystem.h"
#include "Timer.h"
#include "Log.h"
#include "UI/GuardWidget.h"
#include "UI/HealthWidget.h"
#include "Gameplay/GameInstance.h"
#include "Physics/Raycast.h"

Unit::Unit()
{
	isGridObstacle = false;

	battleState.Add(BattleStates::fight);
	battleState.Add(BattleStates::flee);
	battleState.Add(BattleStates::wander);

	memoryOnDeath = MemoryComponent::system.Add(this);
	memoryOnDeath->name = "MemoryOnDeath"; //Seperate the name, GridActor and Unit have two memory components.
}

void Unit::Start()
{
	__super::Start();

	healthWidget = CreateWidget<HealthWidget>();
	healthWidget->healthPoints = health;
	healthWidget->maxHealthPoints = health;

	nextMovePos = GetPositionVector();
}

void Unit::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	healthWidget->pos = GetHomogeneousPositionVector();

	if (isUnitTurn && !attackWindingUp)
	{
		if (XMVector4Equal(nextMovePos, GetPositionVector()))
		{
			if (movementPathNodeIndex < pathNodes.size())
			{
				nextMovePos = XMLoadFloat3(&pathNodes[movementPathNodeIndex]->worldPosition);

				SetUnitLookAt(nextMovePos);

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
	props.Add("Move Speed", &moveSpeed);
	props.Add("Attack Points", &attackPoints);
	props.Add("Attack Range", &attackRange);
	props.Add("Battle State", &battleState);
	props.AddProp(deathText);
	return props;
}

void Unit::InflictDamage(int damage)
{
	if (health <= damage && isDestructible)
	{
		GameInstance::AddGuilt();

		memoryOnDeath->CreateMemory(this->name);

		healthWidget->Destroy();

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

	//Get nodes based on attack range
	std::vector<GridNode*> attackNodes;
	std::vector<GridNode*> closedNodes;
	grid->GetNeighbouringNodes(standingNode, attackNodes);

	for (int rangeIndex = 0; rangeIndex < attackRange; rangeIndex++)
	{
		for (int nodeIndex = 0; nodeIndex < attackNodes.size(); nodeIndex++)
		{
			grid->GetNeighbouringNodes(attackNodes[nodeIndex], closedNodes);
		}

		attackNodes.insert(attackNodes.end(), closedNodes.begin(), closedNodes.end());

		closedNodes.clear();
	}

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

	//Do a raycast towards player. Lets player go behind cover.
	Ray ray(this);
	ray.actorsToIgnore.push_back(player); //Ignore player too. Attack hits if nothing is hit
	if (!Raycast(ray, GetPositionVector(), player->GetPositionVector()))
	{
		//Attack hit
		if (player->guarding)
		{
			GameUtils::PlayAudioOneShot("shield_hit.wav");
		}

		player->InflictDamage(attackPoints);
		Log("%s attacked %s", this->name.c_str(), player->name.c_str());
	}
	else if (ray.hitActor)
	{
		//Attack miss
		Log("[%s] attack missed [%s]. Hit [%s] instead.",
			this->name.c_str(), player->name.c_str(), ray.hitActor->name.c_str());

		auto hitGridActor = dynamic_cast<GridActor*>(ray.hitActor);
		if (hitGridActor)
		{
			hitGridActor->InflictDamage(attackPoints);
		}
	}

	player->nextCameraFOV = 60.f;
	GameUtils::SetActiveCameraTarget(player);
	
	attackWindingUp = false;

	EndTurn();
}

void Unit::ShowUnitMovementPath()
{
	auto grid = GameUtils::GetGrid();
	GridNode* destinationNode = grid->GetNode(0, 0);

	auto previewMovementNodes = GetMovementPathPreviewNodes(destinationNode);

	grid->DisplayHideAllNodes();

	for (auto node : previewMovementNodes)
	{
		node->preview = true;
		node->DisplayShow();
	}
}

void Unit::SetUnitLookAt(XMVECTOR lookAtPoint)
{
	auto lookAtRot = VMath::LookAtRotation(lookAtPoint, GetPositionVector());
	SetRotation(lookAtRot);
}

std::vector<GridNode*> Unit::GetMovementPathPreviewNodes(GridNode* destinationNode)
{
	auto grid = GameUtils::GetGrid();
	grid->ResetAllNodes();

	GridNode* startingNode = grid->GetNode(xIndex, yIndex);

	std::vector<GridNode*> nodes;
	std::vector<GridNode*> attackNodes;
	std::vector<GridNode*> movementNodes;
	std::vector<GridNode*> closedNodes;

	grid->GetNeighbouringNodes(startingNode, movementNodes);

	//MOVEMENT NODES
	for (int moveIndex = 0; moveIndex < movementPoints - 1; moveIndex++)
	{
		for (int previewIndex = 0; previewIndex < movementNodes.size(); previewIndex++)
		{
			grid->GetNeighbouringNodes(movementNodes[previewIndex], closedNodes);
		}

		movementNodes.insert(movementNodes.end(), closedNodes.begin(), closedNodes.end());

		closedNodes.clear();
	}

	//ATTACK NODES

	//Attack nodes will always be on par with num of movement nodes
	attackNodes.insert(attackNodes.end(), movementNodes.begin(), movementNodes.end());

	for (int rangeIndex = 0; rangeIndex < attackRange; rangeIndex++)
	{
		for (int previewIndex = 0; previewIndex < attackNodes.size(); previewIndex++)
		{
			grid->GetNeighbouringNodes(attackNodes[previewIndex], closedNodes);
		}

		attackNodes.insert(attackNodes.end(), closedNodes.begin(), closedNodes.end());

		closedNodes.clear();
	}

	//Set attack node colours first
	for (auto node : attackNodes)
	{
		node->SetColour(GridNode::previewColour);
	}

	for (auto node : movementNodes)
	{
		node->SetColour(GridNode::normalColour);
	}

	//END COMBINED NODES
	nodes.insert(nodes.end(), movementNodes.begin(), movementNodes.end());
	nodes.insert(nodes.end(), attackNodes.begin(), attackNodes.end());

	return nodes;
}
