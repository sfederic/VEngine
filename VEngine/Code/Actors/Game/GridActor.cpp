#include "vpch.h"
#include "GridActor.h"
#include "Actors/Game/Unit.h"
#include "Actors/Game/Player.h"
#include "Components/MeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/Game/DialogueComponent.h"
#include "UI/Game/HealthWidget.h"
#include "Grid.h"
#include "WaterVolume.h"
#include "FenceActor.h"
#include "Core/VMath.h"
#include "Core/Core.h"
#include "Physics/Raycast.h"
#include "Gameplay/GameUtils.h"
#include "Particle/SpriteSheet.h"

GridActor::GridActor()
{
	mesh = CreateComponent("Mesh", MeshComponent("cube.vmesh", "test.png"));
	rootComponent = mesh;

	dialogueComponent = CreateComponent<DialogueComponent>("Dialogue");
}

void GridActor::Create()
{
	__super::Create();

	mesh->SetRenderStatic(false);
}

bool GridActor::CheckNextRotationBoundsIntersect()
{
	if (ignoreRotationValidCheck)
	{
		return false;
	}

	BoundingOrientedBox nextRotBounds = mesh->GetBoundsInWorldSpace();
	XMStoreFloat4(&nextRotBounds.Orientation, nextRot);
	nextRotBounds.Extents.x -= 0.15f;
	nextRotBounds.Extents.y -= 0.15f;
	nextRotBounds.Extents.z -= 0.15f;
	HitResult hit(this);
	return OrientedBoxCast(hit, nextRotBounds, true, true);
}

void GridActor::OnLinkActivate()
{
	isLinked = true;

	//Reset grid nodes with this actor being ignored.
	Grid::system.GetOnlyActor()->RecalcNodesToIgnoreLinkedGridActor(this);
}

void GridActor::OnLinkDeactivate()
{
	isLinked = false;

	//Reset grid nodes. Include this actor again.
	Grid::system.GetOnlyActor()->Awake();
}

void GridActor::OnLinkRotate()
{
	isRotating = true;
	SpawnDustSpriteSheet();
}

void GridActor::CheckSetIsMoving()
{
	if (isMoving)
	{
		if (CheckMovementAndRotationStopped())
		{
			isMoving = false;
		}
	}
}

void GridActor::CheckSetIsRotating()
{
	if (isRotating)
	{
		if (CheckMovementAndRotationStopped())
		{
			isRotating = false;

			if (bigGridActor)
			{
				auto grid = Grid::system.GetOnlyActor();
				HitResult hit;
				//@Todo: don't love this call here because of performance. recalcing nodes isn't
				//toooo slow, but would still be nicer with some sort of boxcast intersecting with node tiles
				//or something. Best I can do here is limit it with a bool for big Grid Actors.
				grid->RecalcAllNodes(hit);
			}
		}
	}
}

void GridActor::OnLinkMove()
{
	isMoving = true;
	SpawnDustSpriteSheet();
}

void GridActor::Inspect()
{
	auto player = Player::system.GetOnlyActor();
	player->SetCameraTargetActor(this);
	player->SetZoomedInCameraFOV();
}

void GridActor::Start()
{
	__super::Start();

	nextPos = GetPositionV();
	nextRot = GetRotationV();

	SetGridPosition();

	if (!IsActive())
	{
		auto node = GetCurrentNode();
		if (node)
		{
			node->Show();
		}
	}
}

void GridActor::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	CheckSetIsMoving();
	CheckSetIsRotating();

	SetPosition(VMath::VectorConstantLerp(GetPositionV(), nextPos, deltaTime, moveSpeed));
	SetRotation(VMath::QuatConstantLerp(GetRotationV(), nextRot, deltaTime, rotateSpeed));

	CheckIfSubmerged();
	if (isSubmerged)
	{
		return;
	}

	if (canFall)
	{
		HitResult hit(this);
		if (!Raycast(hit, GetPositionV(), -VMath::GlobalUpVector(), 0.75f))
		{
			inFall = true;
			constexpr float fallSpeed = 2.f;
			nextPos -= VMath::GlobalUpVector() * fallSpeed * deltaTime;
		}
		else if (inFall)
		{
			inFall = false;
			HitResult nodeHit;
			GetCurrentNode()->RecalcNodeHeight(nodeHit);
			nextPos.m128_f32[1] = std::round(nextPos.m128_f32[1]);
		}
	}

	dialogueComponent->SetPosition(GetHomogeneousPositionV());
}

Properties GridActor::GetProps()
{
	auto props = __super::GetProps();
	props.title = "GridActor";
	props.Add("Destruct", &isDestructible);
	props.Add("Health", &health);
	props.Add("Interact", &isInteractable);
	props.Add("Inspect", &isInspectable);
	props.Add("Interact Text", &interactText);
	props.Add("InteractKnownText", &interactKnownText);
	props.Add("DisableGridInteract", &disableGridInteract);
	props.Add("Obstacle", &isGridObstacle);
	props.Add("Link Rotation", &canBeRotatedInLink);
	props.Add("Link Movement", &canBeMovedInLink);
	props.Add("Ignore Rotate Check", &ignoreRotationValidCheck);
	props.Add("Rotate Yaw", &canBeRotatedYawYAxis);
	props.Add("Rotate Pitch", &canBeRotatedPitchXAxis);
	props.Add("Rotate Roll", &canBeRotatedRollZAxis);
	props.Add("Move Axis Pos.", &validPositiveMovementAxis);
	props.Add("Move Axis Neg.", &validNegativeMovementAxis);
	props.Add("Move Speed", &moveSpeed);
	props.Add("MoveOnlyZAxis", &moveConstrainedToZAxis);
	props.Add("MoveOnlyXAxis", &moveConstrainedToXAxis);
	props.Add("Rotate Speed", &rotateSpeed);
	props.Add("Can Fall", &canFall);
	props.Add("BigGridActor", &bigGridActor);
	return props;
}

void GridActor::InflictDamage(int damage)
{
	Attacked();

	if (!isDestructible) return;

	health -= damage;
	if (healthWidget)
	{
		healthWidget->healthPoints = health;
	}

	if (health <= 0)
	{
		//Hide node if attacked Unit is the last enemy
		if (dynamic_cast<Unit*>(this))
		{
			GetCurrentNode()->Hide();
		}
		else
		{
			GetCurrentNode()->Show();
		}

		HitResult hit(this);
		GetCurrentNode()->RecalcNodeHeight(hit);
		Remove();
	}
}

void GridActor::SetGridPosition()
{
	xIndex = std::round(GetPosition().x);
	yIndex = std::round(GetPosition().z);
}

GridNode* GridActor::GetCurrentNode()
{
	if (disableGridInteract)
	{
		return nullptr;
	}

	auto grid = Grid::system.GetFirstActor();
	auto node = grid->GetNode(xIndex, yIndex);
	return node;
}

void GridActor::RecalcCurrentNodePosition()
{
	auto node = GetCurrentNode();
	HitResult hit(this);
	node->RecalcNodeHeight(hit);
}

void GridActor::RecalcCurrentNodeDontIgnoreThis()
{
	auto node = GetCurrentNode();
	HitResult hit;
	node->RecalcNodeHeight(hit);
}

bool GridActor::CheckNextNodeMoveIsValid(XMVECTOR nextMoveCardinalDirection)
{
	VMath::RoundVector(nextMoveCardinalDirection);

	const int nextXIndex = (int)std::round(nextPos.m128_f32[0]);
	const int nextYIndex = (int)std::round(nextPos.m128_f32[2]);

	auto grid = Grid::system.GetOnlyActor();

	const XMVECTOR currentPos = GetPositionV();

	if (nextXIndex >= grid->GetSizeX() || nextYIndex >= grid->GetSizeY()
		|| nextXIndex < 0 || nextYIndex < 0)
	{
		nextPos = currentPos;
		return false;
	}

	auto nextNodeToMoveTo = grid->GetNode(nextXIndex, nextYIndex);
	if (!nextNodeToMoveTo->active)
	{
		nextPos = currentPos;
		return false;
	}

	auto nextNode = grid->GetNode(nextXIndex, nextYIndex);

	//Check next node height
	if (nextNode->worldPosition.y > (GetPosition().y + Grid::maxHeightMove))
	{
		nextPos = currentPos;
		return false;
	}

	//Check if player is standing on next node
	auto player = Player::system.GetOnlyActor();
	int playerGridX, playerGridY;
	player->GetGridIndices(playerGridX, playerGridY);
	if (nextNode->Equals(playerGridX, playerGridY))
	{
		nextPos = currentPos;
		return false;
	}

	//Fence check
	HitResult fenceHit(this);
	if (Raycast(fenceHit, GetPositionV(), nextMoveCardinalDirection, 1.0f))
	{
		if (fenceHit.GetHitActorAs<FenceActor>())
		{
			Log("[%s] hit fence.", GetName().c_str());
			nextPos = GetPositionV();
			return false;
		}
	}

	//Note that both bools can be set to true, in which case z axis will take precedence.
	if (moveConstrainedToZAxis)
	{
		if (!VMath::VecEqual(nextMoveCardinalDirection, VMath::GlobalForwardVector()) &&
			!VMath::VecEqual(nextMoveCardinalDirection, -VMath::GlobalForwardVector()))
		{
			Log("[%s] constrained to Z axis. Can't be moved in this direction.", GetName().c_str());
			nextPos = GetPositionV();
			return false;
		}
	}
	else if (moveConstrainedToXAxis)
	{
		if (!VMath::VecEqual(nextMoveCardinalDirection, VMath::GlobalRightVector()) &&
			!VMath::VecEqual(nextMoveCardinalDirection, -VMath::GlobalRightVector()))
		{
			Log("[%s] constrained to X axis. Can't be moved in this direction.", GetName().c_str());
			nextPos = GetPositionV();
			return false;
		}
	}

	nextPos = XMLoadFloat3(&nextNode->worldPosition);

	//Make it so that floating actors won't be on the same Y pos as the node.
	if (!canFall)
	{
		nextPos.m128_f32[1] = GetPosition().y;
	}

	//When a GridActor moves, need to recalculate the Grid node height of the last position it was at.
	const int lastGridIndexX = (int)std::round(GetPosition().x);
	const int lastGridIndexY = (int)std::round(GetPosition().z);
	GridNode* lastNode = grid->GetNode(lastGridIndexX, lastGridIndexY);
	HitResult lastPosHit;
	lastPosHit.actorsToIgnore.emplace_back(this);
	lastPosHit.actorsToIgnore.emplace_back(Player::system.GetOnlyActor());
	lastNode->RecalcNodeHeight(lastPosHit);

	return true;
}

ForwardFace GridActor::GetCurrentForwardFace()
{
	const XMFLOAT3 forward = GetForwardVector();

	const int forwardIndex = static_cast<int>(std::lroundf(forward.z));
	const int rightIndex = static_cast<int>(std::lroundf(forward.x));

	if (forwardIndex > 0) return ForwardFace::positiveZ;
	else if (forwardIndex < 0) return ForwardFace::negativeZ;
	else if (rightIndex > 0) return ForwardFace::positiveX;
	else if (rightIndex < 0) return ForwardFace::negativeX;

	throw new std::exception("something went wrong with forward vector");
}

bool GridActor::CheckMovementAndRotationStopped()
{
	return XMVector4Equal(GetPositionV(), nextPos) && XMQuaternionEqual(GetRotationV(), nextRot);
}

void GridActor::RoundPosition()
{
	XMVECTOR pos = GetPositionV();
	VMath::RoundVector(pos);
	SetNextPos(pos);
}

void GridActor::AddNextRot(XMVECTOR axis, float angle)
{
	auto newRotation = XMQuaternionMultiply(nextRot,
		DirectX::XMQuaternionRotationAxis(axis,
			XMConvertToRadians(Core::GetDeltaTime() * angle)));
	SetNextRot(newRotation);
}

void GridActor::SetMeshFilename(std::string_view filename)
{
	mesh->SetMeshFilename(filename);
}

void GridActor::AddNextRotation(XMVECTOR axis, float angle)
{
	nextRot = VMath::AddRotationAngle(nextRot, axis, angle);
}

void GridActor::AddNextPosition(XMVECTOR offset)
{
	nextPos += offset;
}

bool GridActor::IsNextMoveAxisValid(XMVECTOR direction)
{
	const float xAxis = direction.m128_f32[0];
	const float yAxis = direction.m128_f32[2];

	if (xAxis == 1 && validPositiveMovementAxis.x == 0) return false;
	if (xAxis == -1 && validNegativeMovementAxis.x == 0) return false;

	if (yAxis == 1 && validPositiveMovementAxis.y == 0) return false;
	if (yAxis == -1 && validNegativeMovementAxis.y == 0) return false;

	return true;
}

void GridActor::CheckIfSubmerged()
{
	for (auto& waterVolume : WaterVolume::system.GetActors())
	{
		auto pos = GetPositionV();
		if (waterVolume->Contains(pos))
		{
			isSubmerged = true;

			HitResult hit(this);
			//Make sure nothing is above this actor and make sure the water's plane mesh has no backside
			if (!Raycast(hit, GetPositionV(), VMath::GlobalUpVector(), 10.f))
			{
				nextPos.m128_f32[1] = waterVolume->GetPosition().y;
				return;
			}
		}
	}

	isSubmerged = false;
}

void GridActor::DisableAllInteractivity()
{
	isDestructible = false;
	isInspectable = false;
	disableGridInteract = true;
	canBeRotatedInLink = false;
	canBeMovedInLink = false;
	canBeRotatedYawYAxis = false;
	canBeRotatedPitchXAxis = false;
	ignoreRotationValidCheck = true;
}

void GridActor::SpawnDustSpriteSheet()
{
	XMVECTOR spawnPos = GetPositionV();
	spawnPos.m128_f32[1] -= 0.25f;
	auto& sprite = GameUtils::SpawnSpriteSheet("Sprites/dust.png", spawnPos, false, 3, 2);
	sprite.SetAnimationSpeed(15.f);
	sprite.SetWorldScale(0.5f);
}
