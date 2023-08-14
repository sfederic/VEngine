#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"
#include "Gameplay/ForwardFace.h"

struct MeshComponent;
struct HealthWidget;
struct GridNode;
struct DialogueComponent;

//Base struct for every interactive actor that is placed in the level
class GridActor : public Actor
{
public:
	ACTOR_SYSTEM(GridActor);

	MeshComponent* mesh = nullptr;
	HealthWidget* healthWidget = nullptr;
	DialogueComponent* dialogueComponent = nullptr;

protected:
	float moveSpeed = 12.f;
	float rotateSpeed = 12.f;

	//Actor's positions on the level grid
	int xIndex = -1;
	int yIndex = -1;

	int health = 1;

	bool isDestructible = true;

	//Inspection interaction, shows dialogue
	bool isInspectable = true;

	//Used for when the grid actor is off the main grid, ignores GetNode() calls
	bool disableGridInteract = false;

	//Bools to denote whether GridActor can be moved by player's Link ability
	bool canBeRotatedInLink = true;
	bool canBeMovedInLink = true;

	bool canBeRotatedYawYAxis = true;
	bool canBeRotatedPitchXAxis = true;

	bool ignoreRotationValidCheck = false;

	bool isLinked = false;
	bool inFall = false;
	bool canFall = true;
	bool isSubmerged = false;
	bool isRotating = false;
	bool isMoving = true;

	//These two are all the axis valid axis a GridActor can move on.
	//1 or -1 denotes a valid direction (based on the axis type), 0 denotes it can't move in that cardinal direction.
	XMFLOAT2 validPositiveMovementAxis = XMFLOAT2(1.f, 1.f);
	XMFLOAT2 validNegativeMovementAxis = XMFLOAT2(1.f, 1.f);

	XMVECTOR nextPos = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	XMVECTOR nextRot = XMVectorSet(0.f, 0.f, 0.f, 1.f);

public:
	std::wstring interactText;
	std::wstring interactKnownText;

	//Physical interaction, moves/activates actor
	bool isInteractable = true;

	//whether the grid node the gridactor is on is inactive.
	bool isGridObstacle = true;

	GridActor();

	bool CheckNextRotationBoundsIntersect();

	virtual void OnLinkActivate();
	virtual void OnLinkDeactivate();

	virtual void OnLinkRotate() { isRotating = true; }
	virtual void OnLinkRotateRight() {}
	virtual void OnLinkRotateLeft() {}

	virtual void OnRotationEnd();
	virtual void OnMoveEnd();

	virtual void OnLinkMove() { isMoving = true; }

	virtual void Interact() {}

	//For when a fluid or something hits a gridactor (e.g. to extinguish flames)
	virtual void Douse() {}

	//Like when a steam valve or fire or whatever something hits the actor.
	virtual void Steamed() {}

	//When exposed to something on fire
	virtual void Burn() {}

	//When in range of a crystal providing electrical power
	virtual void PowerUp() {}

	//When grid actor leaves influence of crystal
	virtual void PowerDown() {}

	//A physical action where the grid actor is cut
	virtual void Slice() {}

	//Called when this GridActor is attacked.
	virtual void Attacked() {}

	virtual void EndInteract() {}
	virtual void Start() override;
	virtual void Tick(float deltaTime) override;
	virtual Properties GetProps() override;
	virtual void InflictDamage(int damage);

	//Sets x and y indices on battlegrid for gridactor
	void SetGridPosition();
	
	//returns the node the gridactor is currently on.
	GridNode* GetCurrentNode();
	void RecalcCurrentNodePosition();

	bool CheckNextNodeMoveIsValid();

	//Get forward face in grid terms based on forward vector and current grid position.
	ForwardFace GetCurrentForwardFace();

	bool CheckMovementAndRotationStopped();

	XMVECTOR GetNextRot() { return nextRot; }
	XMVECTOR GetNextPos() { return nextPos; }
	void SetNextRot(XMVECTOR rot) { nextRot = rot; }
	void SetNextPos(XMVECTOR pos) { nextPos = pos; }

	void SetMeshFilename(std::string_view filename);

	void AddNextRotation(XMVECTOR axis, float angle);
	void AddNextPosition(XMVECTOR offset);

	bool IsNextMoveAxisValid(XMVECTOR direction);

	bool IsLinked() { return isLinked; }
	bool CanBeMovedInLink() { return canBeMovedInLink; }
	bool CanBeRotatedInLink() { return canBeRotatedInLink; }
	bool CanBeRotatedYawYAxis() { return canBeRotatedYawYAxis; }
	bool CanBeRotatedPitchXAxis() { return canBeRotatedPitchXAxis; }

	void SetCanFall(bool fall) { canFall = fall; }

	void CheckIfSubmerged();

	void DisableAllInteractivity();
};
