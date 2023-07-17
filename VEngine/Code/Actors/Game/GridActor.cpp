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
#include "Core/VMath.h"
#include "Physics/Raycast.h"
#include "Gameplay/GameUtils.h"

GridActor::GridActor()
{
	mesh = CreateComponent("Mesh", MeshComponent("cube.vmesh", "test.png"));
	rootComponent = mesh;

	dialogueComponent = CreateComponent<DialogueComponent>("Dialogue");
}

bool GridActor::CheckNextRotationBoundsIntersect()
{
	if (ignoreRotationValidCheck)
	{
		return false;
	}

	BoundingOrientedBox nextRotBounds = mesh->GetBoundsInWorldSpace();
	XMStoreFloat4(&nextRotBounds.Orientation, nextRot);
	nextRotBounds.Extents.x -= 0.1f;
	nextRotBounds.Extents.y -= 0.1f;
	nextRotBounds.Extents.z -= 0.1f;
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

void GridActor::OnRotationEnd()
{
	if (isRotating)
	{
		if (CheckMovementAndRotationStopped())
		{
			isRotating = false;
		}
	}
}

void GridActor::OnMoveEnd()
{
	if (isMoving)
	{
		if (CheckMovementAndRotationStopped())
		{
			isMoving = false;
		}
	}
}

void GridActor::Start()
{
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
	OnMoveEnd();
	OnRotationEnd();

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
	props.Add("Move Axis Pos.", &validPositiveMovementAxis);
	props.Add("Move Axis Neg.", &validNegativeMovementAxis);
	props.Add("Move Speed", &moveSpeed);
	props.Add("Rotate Speed", &rotateSpeed);
	props.Add("Can Fall", &canFall);
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
		Destroy();
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

bool GridActor::CheckNextNodeMoveIsValid()
{
	const int nextXIndex = (int)std::round(nextPos.m128_f32[0]);
	const int nextYIndex = (int)std::round(nextPos.m128_f32[2]);

	auto grid = Grid::system.GetOnlyActor();

	if (nextXIndex >= grid->sizeX || nextYIndex >= grid->sizeY
		|| nextXIndex < 0 || nextYIndex < 0)
	{
		nextPos = GetPositionV();
		return false;
	}

	auto nextNodeToMoveTo = grid->GetNode(nextXIndex, nextYIndex);
	if (!nextNodeToMoveTo->active)
	{
		nextPos = GetPositionV();
		return false;
	}

	auto nextNode = grid->GetNode(nextXIndex, nextYIndex);

	//Check next node height
	if (nextNode->worldPosition.y > (GetPosition().y + Grid::maxHeightMove))
	{
		nextPos = GetPositionV();
		return false;
	}

	nextPos = XMLoadFloat3(&nextNode->worldPosition);

	//When a GridActor moves, need to recalculate the Grid node height of the last position it was at.
	const int lastGridIndexX = (int)std::round(GetPosition().x);
	const int lastGridIndexY = (int)std::round(GetPosition().z);
	GridNode* lastNode = grid->GetNode(lastGridIndexX, lastGridIndexY);
	HitResult lastPosHit;
	lastPosHit.actorsToIgnore.push_back(this);
	lastPosHit.actorsToIgnore.push_back(Player::system.GetOnlyActor());
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
	if (!isSubmerged)
	{
		bool waterVolumeMultipleCheck = false;

		for (auto& waterVolume : WaterVolume::system.GetActors())
		{
			auto pos = GetPositionV();
			if (waterVolume->Contains(pos))
			{
				assert(!waterVolumeMultipleCheck && "GridActor is within more than one water volume.");
				nextPos = waterVolume->GetPositionV(); //This should give a nice little water 'bob' visual.
				isSubmerged = true;
				moveSpeed /= 2.f; //Slow down movement speed when in water
				waterVolumeMultipleCheck = true;
				return;
			}
		}
	}
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
