#include "vpch.h"
#include "Unit.h"
#include <algorithm>
#include "Gameplay/GridNode.h"
#include "Gameplay/GameUtils.h"
#include "Grid.h"
#include "Components/MeshComponent.h"
#include "Components/BoxTriggerComponent.h"
#include "Components/Game/MemoryComponent.h"
#include "VMath.h"
#include "Player.h"
#include "Gameplay/BattleSystem.h"
#include "Timer.h"
#include "Log.h"
#include "UI/Game/GuardWidget.h"
#include "UI/Game/HealthWidget.h"
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

	healthWidget = CreateWidget<HealthWidget>();
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

	healthWidget->pos = GetHomogeneousPositionV();

	if (isUnitTurn && !attackWindingUp)
	{
		if (XMVector4Equal(nextMovePos, GetPositionV()))
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

				auto player = Player::system.GetFirstActor();

				if (Attack())
				{
					//deal with attack wind up
					attackWindingUp = true;

					player->guardWidget->AddToViewport();
					player->ableToGuard = true;
					player->nextCameraFOV = 30.f;
					player->unitCurrentlyAttackingPlayer = this;

					SetAttackDirection();

					GameUtils::SetActiveCameraTarget(this);

					Timer::SetTimer(2.f, std::bind(&Unit::WindUpAttack, this));
				}
				else
				{
					//@Todo: come back and see if skills on units are worth keeping. Might be too much work.
					//Use a skill on turn end if target is out of range of normal attack.
					/*if (IsTargetInRangeOfSkills(player->GetCurrentNode()))
					{
						skills["line"]->SetNodesForSkillRange(GetCurrentNode(), player->GetCurrentNode());
					}*/

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
	props.AddProp(actorToFocusOn);
	props.AddProp(numOfAttacks);
	props.AddProp(deathText);
	return props;
}

void Unit::InflictDamage(int damage)
{
	if (health <= damage && isDestructible)
	{
		GameInstance::AddGuilt();

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

	currentAttackNumber = numOfAttacks;

	GetCurrentNode()->Show();

	auto player = Player::system.GetFirstActor();

	if (battleState.Compare(BattleStates::fight))
	{
		//Move towards player to attack
		MoveToNode(player->xIndex, player->yIndex);
	}
	else if (battleState.Compare(BattleStates::evade))
	{
		//Move away from player to evade
		MoveToNode(player->xIndex, player->yIndex);
	}
	else if (battleState.Compare(BattleStates::escape))
	{
		//int is the index into EntranceTrigger actor system vector
		std::vector<std::pair<float, int>> entranceDistances;

		//Find entrance closest to unit and move to it
		for (int i = 0; i < EntranceTrigger::system.GetActors().size(); i++)
		{
			auto entrance = EntranceTrigger::system.GetActors()[i];
			float dist = XMVector3Length(entrance->GetPositionV() - this->GetPositionV()).m128_f32[0];
			entranceDistances.push_back(std::make_pair(dist, i));
		}

		//Sort by distance
		std::sort(entranceDistances.begin(), entranceDistances.end());
		auto entranceTriggerToMoveTo = EntranceTrigger::system.GetActors()[entranceDistances.front().second];

		entranceToEscapeTo = entranceTriggerToMoveTo;

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

	auto player = Player::system.GetFirstActor();
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
	auto player = Player::system.GetFirstActor();

	//Do a raycast towards player. Lets player go behind cover.
	Ray ray(this);
	ray.actorsToIgnore.push_back(player); //Ignore player too. Attack hits if nothing is hit
	if (!Raycast(ray, GetPositionV(), player->GetPositionV()))
	{
		if (attackDirection == player->defendDirection && player->guarding) //Successful defend
		{
			GameUtils::CameraShake(1.f);
			GameUtils::PlayAudioOneShot("shield_hit.wav");
		}
		else //Attack Hit
		{
			GameUtils::CameraShake(1.f);
			GameUtils::PlayAudioOneShot("sword_hit.wav");
		}

		player->InflictDamage(attackPoints);

		Log("%s attacked %s", this->GetName().c_str(), player->GetName().c_str());
	}
	else if (ray.hitActor)
	{
		//Attack miss
		Log("[%s] attack missed [%s]. Hit [%s] instead.",
			this->GetName().c_str(), player->GetName().c_str(), ray.hitActor->GetName().c_str());

		auto hitGridActor = dynamic_cast<GridActor*>(ray.hitActor);
		if (hitGridActor)
		{
			hitGridActor->InflictDamage(attackPoints);
		}
	}

	player->ResetGuard();

	currentAttackNumber--;

	if (currentAttackNumber > 0) //Attack again
	{
		SetAttackDirection();
		Timer::SetTimer(2.f, std::bind(&Unit::WindUpAttack, this));
	}
	else //End turn
	{
		player->nextCameraFOV = 60.f;
		GameUtils::SetActiveCameraTarget(player);

		attackWindingUp = false;

		player->ableToGuard = false;
		player->guardWidget->RemoveFromViewport();
		player->unitCurrentlyAttackingPlayer = nullptr;

		EndTurn();
	}
}

void Unit::SetAttackDirection()
{
	attackDirection = GetRandomAttackDirection();
	Player::system.GetFirstActor()->guardWidget->attackingUnitAttackDirection = attackDirection;
}

AttackDirection Unit::GetRandomAttackDirection()
{
	 return static_cast<AttackDirection>(rand() % (int)AttackDirection::Count);
}

int Unit::GetHighestSkillRange()
{
	int highestRange = 0;

	for (auto& skillIt : skills)
	{
		auto& skill = skillIt.second;
		const int skillRange = skill->GetRange();
		if (skillRange > highestRange)
		{
			highestRange = skillRange;
		}
	}

	return highestRange;
}

bool Unit::IsTargetInRangeOfSkills(GridNode* targetNode)
{
	for (auto& skillIt : skills)
	{
		auto& skill = skillIt.second;
		if (skill->IsTargetNodeInRange(GetCurrentNode(), targetNode))
		{
			return true;
		}
	}

	return false;
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
