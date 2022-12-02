#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"

struct MeshComponent;
struct PlayerActionBarWidget;
struct GuardWidget;
struct GridNode;

//Base class for all player controllable units.
//Should never be instantiated, no need for ACTOR_SYSTEM.
class PlayerUnit : public Actor
{
public:
	void CheckNextMoveNode(XMVECTOR previousPos);
	GridNode* GetCurrentNode();

protected:
	void ExpendActionPoints(int num);
	void PreviewMovementNodesDuringBattle();

public:
	XMVECTOR nextPos;
	XMVECTOR nextRot;

	PlayerActionBarWidget* actionBarWidget = nullptr;
	GuardWidget* guardWidget = nullptr;

	MeshComponent* mesh = nullptr;

	int actionPoints = 5;

private:
	int xIndex = -1;
	int yIndex = -1;
};
