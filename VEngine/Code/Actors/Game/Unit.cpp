#include "vpch.h"
#include "Unit.h"
#include <algorithm>
#include "Gameplay/GridNode.h"
#include "Gameplay/GameUtils.h"
#include "Grid.h"
#include "Components/MeshComponent.h"
#include "Components/BoxTriggerComponent.h"
#include "Components/Game/MemoryComponent.h"
#include "Core/VMath.h"
#include "Player.h"
#include "Gameplay/BattleSystem.h"
#include "Gameplay/BattleCards/TrapCard.h"
#include "Core/Timer.h"
#include "Core/Log.h"
#include "UI/UISystem.h"
#include "UI/Game/HealthWidget.h"
#include "UI/Game/MemoryGainedWidget.h"
#include "UI/Game/ActivateTrapWidget.h"
#include "Gameplay/GameInstance.h"
#include "Actors/Game/EntranceTrigger.h"
#include "Physics/Raycast.h"

Unit::Unit()
{
	isGridObstacle = false;

	battleState.Add(BattleStates::fight);
	battleState.Add(BattleStates::evade);
	battleState.Add(BattleStates::escape);
	battleState.Add(BattleStates::wander);

	memoryOnDeath = MemoryComponent::system.Add("MemoryOnDeath", this);
	memoryOnDeath->name = "MemoryOnDeath"; //Seperate the name, GridActor and Unit have two memory components.

	//intentBeam = Polyboard::system.Add(this);
}

void Unit::Start()
{
	__super::Start();

	//intentBeam->GenerateVertices();

	healthWidget = UISystem::CreateWidget<HealthWidget>();
	healthWidget->healthPoints = health;
	healthWidget->maxHealthPoints = health;

	nextMovePos = GetPositionV();

	if (IsActive())
	{
		auto node = GetCurrentNode();
		node->Hide();
	}
}

void Unit::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	//@Todo: figure out how to do intent beams better for units.
	//intentBeam->startPoint = GetPosition();
	//Actor* intentActor = world.GetActorByNameAllowNull(actorToFocusOn);
	//if (intentActor)
	//{
	//	intentBeam->endPoint = intentActor->GetPosition();
	//}

	healthWidget->worldPosition = GetHomogeneousPositionV();

	if (isUnitTurn && !attackWindingUp && !isInTrapNode)
	{
		if (XMVector4Equal(nextMovePos, GetPositionV()))
		{
			if (movementPathNodeIndex < pathNodes.size())
			{
				nextMovePos = XMLoadFloat3(&pathNodes[movementPathNodeIndex]->worldPosition);

				SetUnitLookAt(nextMovePos);

				xIndex = pathNodes[movementPathNodeIndex]->xIndex;
				yIndex = pathNodes[movementPathNodeIndex]->yIndex;

				//Trap node logic
				auto currentNode = GetCurrentNode();
				if (currentNode->trapCard)
				{
					//@Todo: this has the problem where if there are two traps adjacent, the unit will enter a loop
					isInTrapNode = true;
					auto activateTrapWidget = UISystem::CreateWidget<ActivateTrapWidget>();
					activateTrapWidget->AddToViewport();
					activateTrapWidget->SetLinkedUnit(this);
					activateTrapWidget->SetLinkedTrapNode(currentNode->trapCard);
				}

				movementPathNodeIndex++;
			}
			else if (movementPathNodeIndex >= pathNodes.size())
			{
				movementPathNodeIndex = 0;
				pathNodes.clear();

				GetCurrentNode()->Hide();

				auto player = Player::system.GetFirstActor();

				if (Attack())
				{
					//deal with attack wind up
					attackWindingUp = true;

					player->nextCameraFOV = 30.f;

					GameUtils::SetActiveCameraTarget(this);

					Timer::SetTimer(2.f, std::bind(&Unit::WindUpAttack, this));
				}
				else
				{
					EndTurn();

					//Destroy Unit if its escaping and within its entrancetrigger to escape with
					if (battleState.Compare(BattleStates::escape) && entranceToEscapeTo)
					{
						if (entranceToEscapeTo->trigger->Contains(GetPositionV()))
						{
							battleSystem.RemoveUnit(this);
							GetCurrentNode()->Show();
							Log("Unit [%s] escaped through [%s].",
								this->GetName().c_str(), entranceToEscapeTo->GetName().c_str());
							Destroy();
							return;
						}
					}
				}
			}
		}
	}

	SetPosition(VMath::VectorConstantLerp(GetPositionV(), nextMovePos, deltaTime, moveSpeed));
}

Properties Unit::GetProps()
{
	auto props = __super::GetProps();
	props.Add("Move Points", &movementPoints);
	props.Add("Move Speed", &moveSpeed);
	props.Add("Attack Points", &attackPoints);
	props.Add("Attack Range", &attackRange);
	props.Add("Battle State", &battleState);
	props.Add("Focus Actor", &actorToFocusOn);
	props.Add("Num Attacks", &numOfAttacks);
	props.Add("Death Text", &deathText);
	return props;
}

void Unit::InflictDamage(int damage)
{
	if (!battleSystem.isBattleActive)
	{
		battleSystem.StartBattle();
	}

	if (health <= damage && isDestructible)
	{
		memoryOnDeath->CreateMemory(this->GetName());

		healthWidget->Destroy();

		battleSystem.RemoveUnit(this);
	}

	__super::InflictDamage(damage);
}

void Unit::MoveToNode(GridNode* destinationNode)
{
	if (movementPoints <= 0)
	{
		return;
	}

	auto grid = Grid::system.GetFirstActor();
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

	//Move to node furthest away from destination
	if (battleState.Compare(BattleStates::evade))
	{
		float highestHCost = 0.f;
		for (int i = 0; i < movementPathNodes.size(); i++)
		{
			if (movementPathNodes[i]->hCost > highestHCost)
			{
				highestHCost = movementPathNodes[i]->hCost;
				highestHCostIndex = i;
				nextNode = movementPathNodes[i];
			}
		}
	}
	else //Move towards destination
	{
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
	auto grid = Grid::system.GetFirstActor();
	GridNode* destinationNode = grid->GetNode(x, y);

	MoveToNode(destinationNode);
}

void Unit::StartTurn()
{
	isUnitTurn = true;

	GetCurrentNode()->Show();

	auto target = FindClosestPlayerUnit();

	if (battleState.Compare(BattleStates::fight))
	{
		//Move towards player to attack
		MoveToNode(target->xIndex, target->yIndex);
	}
	else if (battleState.Compare(BattleStates::evade))
	{
		//Move away from player to evade
		MoveToNode(target->xIndex, target->yIndex);
	}
	else if (battleState.Compare(BattleStates::escape))
	{
		//int is the index into EntranceTrigger actor system vector
		std::vector<std::pair<float, int>> entranceDistances;

		//Find entrance closest to unit and move to it
		for (int i = 0; i < EntranceTrigger::system.GetActors().size(); i++)
		{
			auto& entrance = EntranceTrigger::system.GetActors()[i];
			float dist = XMVector3Length(entrance->GetPositionV() - this->GetPositionV()).m128_f32[0];
			entranceDistances.push_back(std::make_pair(dist, i));
		}

		//Sort by distance
		std::sort(entranceDistances.begin(), entranceDistances.end());
		auto& entranceTriggerToMoveTo = EntranceTrigger::system.GetActors()[entranceDistances.front().second];

		entranceToEscapeTo = entranceTriggerToMoveTo.get();

		//EntranceTrigger isn't a grid actor, just move to its world position
		int xIndex = std::round(entranceTriggerToMoveTo->GetPosition().x);
		int yIndex = std::round(entranceTriggerToMoveTo->GetPosition().y);
		MoveToNode(xIndex, yIndex);
	}
}

void Unit::EndTurn()
{
	isUnitTurn = false;
	battleSystem.MoveToNextTurn();
}

bool Unit::Attack()
{
	auto standingNode = GetCurrentNode();
	auto grid = Grid::system.GetFirstActor();

	//Get nodes based on attack range
	std::vector<GridNode*> attackNodes;
	std::vector<GridNode*> closedNodes;

	for (int rangeIndex = 0; rangeIndex < attackRange; rangeIndex++)
	{
		for (int nodeIndex = 0; nodeIndex < attackNodes.size(); nodeIndex++)
		{
			grid->GetNeighbouringNodes(attackNodes[nodeIndex], closedNodes);
		}

		attackNodes.insert(attackNodes.end(), closedNodes.begin(), closedNodes.end());

		closedNodes.clear();
	}

	auto target = FindClosestPlayerUnit();
	auto targetNode = target->GetCurrentNode();

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
	auto target = FindClosestPlayerUnit();
	GameUtils::SetActiveCameraTarget(target);
	GameUtils::SpawnSpriteSheet("Sprites/explosion.png", target->GetPositionV(), false, 4, 4);
	target->InflictDamage(attackPoints);

	Player::system.GetFirstActor()->nextCameraFOV = 60.f;

	attackWindingUp = false;

	EndTurn();
}

PlayerUnit* Unit::FindClosestPlayerUnit()
{
	std::multimap<float, PlayerUnit*> distanceMap;
	for (auto playerUnit : World::GetAllActorsOfTypeInWorld<PlayerUnit>())
	{
		distanceMap.emplace(XMVector3Length(playerUnit->GetPositionV() - GetPositionV()).m128_f32[0], playerUnit);
	}
	return distanceMap.begin()->second;
}

void Unit::ShowUnitMovementPath()
{
	auto grid = Grid::system.GetFirstActor();
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
	auto lookAtRot = VMath::LookAtRotation(lookAtPoint, GetPositionV());
	SetRotation(lookAtRot);
}

void Unit::SetMovePathIndexToMax()
{
	movementPathNodeIndex = pathNodes.size();
}

std::vector<GridNode*> Unit::GetMovementPathPreviewNodes(GridNode* destinationNode)
{
	auto grid = Grid::system.GetFirstActor();
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
