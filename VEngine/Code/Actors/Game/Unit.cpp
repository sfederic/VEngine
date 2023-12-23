#include "vpch.h"
#include "Unit.h"
#include "Gameplay/GridNode.h"
#include "Gameplay/GameUtils.h"
#include "Grid.h"
#include "Core/VMath.h"
#include "Core/Camera.h"
#include "Player.h"
#include "Core/Timer.h"
#include "UI/UISystem.h"
#include "UI/Game/HealthWidget.h"

Unit::Unit()
{
	isGridObstacle = false;

	battleState.Add(BattleStates::fight);
	battleState.Add(BattleStates::evade);
	battleState.Add(BattleStates::wander);
}

void Unit::Start()
{
	__super::Start();

	healthWidget = UISystem::CreateWidget<HealthWidget>();
	healthWidget->healthPoints = health;
	healthWidget->maxHealthPoints = health;

	if (IsActive())
	{
		auto node = GetCurrentNode();
		node->Hide();
	}
}

void Unit::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	healthWidget->worldPosition = GetHomogeneousPositionV();

	if (VMath::VecEqual(nextPos, GetPositionV()))
	{
		if (movementPathNodeIndex < pathNodes.size())
		{
			nextPos = XMLoadFloat3(&pathNodes[movementPathNodeIndex]->worldPosition);

			SetUnitLookAt(nextPos);

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
				player->SetZoomedInCameraFOV();
				Camera::SetActiveCameraTarget(this);
				Timer::SetTimer(2.f, std::bind(&Unit::WindUpAttack, this));
			}
		}
	}
}

Properties Unit::GetProps()
{
	auto props = __super::GetProps();
	props.Add("Move Points", &movementPoints);
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
	if (health <= damage && isDestructible)
	{
		healthWidget->Destroy();
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

		movementPathNodes.emplace_back(node);
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
	if (nextNode != nullptr)
	{
		pathNodes.emplace_back(nextNode);
	}

	if (nextNode)
	{
		while (nextNode != startingNode)
		{
			if (nextNode->parentNode)
			{
				nextNode = nextNode->parentNode;
				pathNodes.emplace_back(nextNode);
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

void Unit::MoveToRandomNode()
{
	auto grid = Grid::system.GetOnlyActor();
	int x = VMath::RandomRangeInt(0, grid->GetSizeX());
	int y = VMath::RandomRangeInt(0, grid->GetSizeY());
	GridNode* destinationNode = grid->GetNode(x, y);
	MoveToNode(destinationNode);
}

void Unit::StartTurn()
{
	GetCurrentNode()->Show();

	auto target = Player::system.GetOnlyActor();

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
}

bool Unit::Attack()
{
	auto standingNode = GetCurrentNode();
	auto grid = Grid::system.GetFirstActor();

	//Get nodes based on attack range
	std::vector<GridNode*> attackNodes;
	std::vector<GridNode*> closedNodes;

	assert(attackRange > 0);
	if (attackRange == 1)
	{
		grid->GetNeighbouringNodes(standingNode, attackNodes);
	}
	else
	{
		for (int rangeIndex = 0; rangeIndex < attackRange; rangeIndex++)
		{
			for (int nodeIndex = 0; nodeIndex < attackNodes.size(); nodeIndex++)
			{
				grid->GetNeighbouringNodes(attackNodes[nodeIndex], closedNodes);
			}

			attackNodes.insert(attackNodes.end(), closedNodes.begin(), closedNodes.end());

			closedNodes.clear();
		}
	}

	auto target = Player::system.GetOnlyActor();
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
	auto target = Player::system.GetOnlyActor();
	Camera::SetActiveCameraTarget(target);
	GameUtils::SpawnSpriteSheet("Sprites/blade_slash.png", target->GetPositionV(), false, 3, 5);
	GameUtils::PlayAudioOneShot("armor_light.wav");

	auto player = Player::system.GetOnlyActor();
	player->SetDefaultCameraFOV();
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
	nextRot = lookAtRot;
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
