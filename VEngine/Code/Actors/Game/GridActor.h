#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"
#include "Gameplay/ForwardFace.h"

class MeshComponent;
class HealthWidget;
struct GridNode;
class DialogueComponent;

//@Todo: SkeletalMeshGridActor

//Base struct for every interactive actor that is placed in the level
class GridActor : public Actor
{
public:
	ACTOR_SYSTEM(GridActor);

protected:
	std::wstring interactText;
	std::wstring interactKnownText;

	std::string linkMoveAudio = "cloth_heavy.wav";
	std::string linkRotateAudio = "chainmail1.wav";

	MeshComponent* mesh = nullptr;
	HealthWidget* healthWidget = nullptr;
	DialogueComponent* dialogueComponent = nullptr;

	//For any grid actor that will move/rotate another grid actor on a player's link, 
	//this is the actor the player's camera will focus on instead of itself.
	Actor* actorForPlayerFocusOnLink = nullptr;

	//These two are all the axis valid axis a GridActor can move on.
	//1 or -1 denotes a valid direction (based on the axis type), 0 denotes it can't move in that cardinal direction.
	XMFLOAT2 validPositiveMovementAxis = XMFLOAT2(1.f, 1.f);
	XMFLOAT2 validNegativeMovementAxis = XMFLOAT2(1.f, 1.f);

	XMVECTOR nextPos = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	XMVECTOR nextRot = XMVectorSet(0.f, 0.f, 0.f, 1.f);

	XMVECTOR nextMoveCardinalDirection = XMVectorZero();

	float moveSpeed = 12.f;
	float rotateSpeed = 12.f;
	float fallSpeed = 4.f;

	//Actor's positions on the level grid
	int xIndex = -1;
	int yIndex = -1;

	int health = 1;

	bool isDestructible = true;

	//Inspection interaction, shows dialogue
	bool isInspectable = false;

	//Used for when the grid actor is off the main grid, ignores GetNode() calls
	bool disableGridInteract = false;

	//Bools to denote whether GridActor can be moved by player's Link ability
	bool canBeRotatedInLink = true;
	bool canBeMovedInLink = true;

	bool canBeRotatedYawYAxis = true;
	bool canBeRotatedPitchXAxis = true;

	//Constrain movement to an axis.
	bool moveConstrainedToZAxis = false;
	bool moveConstrainedToXAxis = false;

	//This bool is to denote larger grid actors that need multiple or all nodes on the grid recalculated
	//every time this particular grid actor is moved or rotated.
	bool bigGridActor = false;

	//This bool is for rotating grid actors (e.g. actors stuck on walls) on their z-axis. Because the controls for rotation need
	//to be simple (previous player input had this when holding down shift), some actors will be using this bool
	//to denote that left and right will rotate along the z-axis instead. 
	//Note that this will override yaw (y-axis) rotations.
	bool canBeRotatedRollZAxis = false;

	bool ignoreRotationValidCheck = false;

	bool isLinked = false;
	bool inFall = false;
	bool canFall = true;
	bool isSubmerged = false;
	bool isRotating = false;
	bool isMoving = true;

	void SetPlayerFocusGridActor(Actor* focusActor) { actorForPlayerFocusOnLink = focusActor; }

public:
	//Physical interaction, moves/activates actor
	bool isInteractable = true;

	//whether the grid node the gridactor is on is inactive.
	bool isGridObstacle = true;

	GridActor();

	void Create() override;

	bool CheckNextRotationBoundsIntersect();

	virtual void OnLinkActivate();
	virtual void OnLinkDeactivate();

	virtual void OnLinkRotate();
	virtual void OnLinkRotateRight() {}
	virtual void OnLinkRotateLeft() {}

	virtual void OnRotationEnd() {}
	virtual void OnMoveEnd() {}

	//When the player's facing vector is point towwards this grid actor. Think of it as a 3D "mouse-over".
	virtual void OnPlayerLinkHover() {}
	virtual void OnPlayerLinkHoverOff() {}

	virtual void OnLinkMove();
	virtual void OnLinkMoveForward() {}
	virtual void OnLinkMoveBack() {}
	virtual void OnLinkMoveLeft() {}
	virtual void OnLinkMoveRight() {}

	virtual void Interact() {}

	//Zooms in on the actor
	virtual void Inspect();

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
	void DisableCurrentNode();
	void EnableCurrentNode();
	void RecalcCurrentNodePosition();
	void RecalcCurrentNodeDontIgnoreThis();

	bool CheckNextNodeMoveIsValid(const XMVECTOR nextMoveDirection);

	//Get forward face in grid terms based on forward vector and current grid position.
	ForwardFace GetCurrentForwardFace();

	bool CheckMovementAndRotationStopped();

	//This will pop the grid actor aligned onto the grid in world space if it's out.
	void RoundPosition();

	XMVECTOR GetNextRot() const { return nextRot; }
	XMVECTOR GetNextPos() const { return nextPos; }
	void SetNextRot(XMVECTOR rot) { nextRot = rot; }
	void AddNextRot(XMVECTOR axis, float angle);
	void SetNextPos(XMVECTOR pos) { nextPos = pos; }
	void ResetNextPos();

	void SetMeshFilename(std::string_view filename);

	void AddNextRotation(XMVECTOR axis, float angle);
	void AddNextPosition(XMVECTOR offset);

	bool IsNextMoveAxisValid(XMVECTOR direction);

	bool IsLinked() const { return isLinked; }
	bool CanBeMovedInLink() const { return canBeMovedInLink; }
	bool CanBeRotatedInLink() const { return canBeRotatedInLink; }
	bool CanBeRotatedYawYAxis() const { return canBeRotatedYawYAxis; }
	bool CanBeRotatedPitchXAxis() const { return canBeRotatedPitchXAxis; }
	bool CanBeRotatedRollZAxis() const { return canBeRotatedRollZAxis; }

	void SetCanFall(bool fall) { canFall = fall; }
	bool GetCanFall() const { return canFall; }

	void CheckIfSubmerged();
	void DisableAllInteractivity();
	auto GetPlayerFocusActor() { return actorForPlayerFocusOnLink; }
	bool IsInspectable() const { return isInspectable; }
	auto GetInteractText() { return interactText; }

	//Note that this is the main mesh of the grid actor. Any link effects need to use this mesh.
	auto& GetMesh() { return *mesh; }

	auto GetDialogueComponent() { return dialogueComponent; }

private:
	void SpawnDustSpriteSheet() const;

	void CheckSetIsMoving();
	void CheckSetIsRotating();

	void FallCheck(float deltaTime);
};
