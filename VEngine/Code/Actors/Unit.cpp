#include "Unit.h"
#include "GridNode.h"
#include "GameUtils.h"

void Unit::MoveTo(GridNode& destinationNode)
{
	//BattleGrid* battleGrid = GameUtils::GetBattleGrid();
	//GridNode* startingNode = battleGrid->GetNode(xIndex, yIndex);

	////Assign all costs
	//for (int i = 0; i < movementPathNodes.Num(); i++)
	//{
	//	movementPathNodes[i]->gCost = FVector::Distance(startingNode->location, movementPathNodes[i]->location);
	//	movementPathNodes[i]->hCost = FVector::Distance(destinationNode->location, movementPathNodes[i]->location);
	//}

	////Find lowest distance to 'end' based on UnitState
	//int highestHCostIndex = 0;
	//int lowestHCostIndex = 0;

	//if (unitState == EUnitState::Chase)
	//{
	//	float lowestHCost = TNumericLimits<float>::Max();
	//	for (int i = 0; i < movementPathNodes.Num(); i++)
	//	{
	//		if (movementPathNodes[i]->hCost < lowestHCost)
	//		{
	//			lowestHCost = movementPathNodes[i]->hCost;
	//			lowestHCostIndex = i;
	//		}
	//	}
	//}
	//else if (unitState == EUnitState::Flee)
	//{
	//	float highestHCost = -1.f;
	//	for (int i = 0; i < movementPathNodes.Num(); i++)
	//	{
	//		if (movementPathNodes[i]->hCost >= highestHCost)
	//		{
	//			highestHCost = movementPathNodes[i]->hCost;
	//			highestHCostIndex = i;
	//		}
	//	}
	//}

	//FGridNode* nextNode = nullptr;
	//switch (unitState)
	//{
	//case EUnitState::Stationary: nextNode = startingNode; break;
	//case EUnitState::Chase:	nextNode = movementPathNodes[lowestHCostIndex]; break;
	//case EUnitState::Flee: nextNode = movementPathNodes[highestHCostIndex]; break;
	//case EUnitState::Wander:
	//	int randomNodeIndex = FMath::RandRange(0, movementPathNodes.Num() - 1);
	//	nextNode = movementPathNodes[randomNodeIndex];
	//	break;
	//}

	//while (nextNode != startingNode)
	//{
	//	if (nextNode->parentNode)
	//	{
	//		nextNode = nextNode->parentNode;
	//		pathNodes.Add(nextNode);
	//	}
	//}

	//Algo::Reverse(pathNodes);
	//movementPathNodes.Empty();

	////Activate previous standing node 
	//if (pathNodes.Num() > 0)
	//{
	//	battleGrid->UnhideNode(battleGrid->GetNode(xIndex, yIndex));
	//}
}

Unit::Unit()
{
}

Properties Unit::GetProps()
{
	return Properties();
}
