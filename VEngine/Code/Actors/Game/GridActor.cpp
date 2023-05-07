#include "vpch.h"
#include "GridActor.h"
#include "Actors/Game/Unit.h"
#include "Components/MeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/Game/DialogueComponent.h"
#include "UI/Game/HealthWidget.h"
#include "Grid.h"
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
	BoundingOrientedBox nextRotBounds = mesh->GetBoundingBox();
	VMath::UpdateBoundingBox(nextRotBounds, this);
	XMStoreFloat4(&nextRotBounds.Orientation, nextRot);
	nextRotBounds.Extents.x -= 0.1f;
	nextRotBounds.Extents.y -= 0.1f;
	nextRotBounds.Extents.z -= 0.1f;
	HitResult hit(this);
	return OrientedBoxCast(hit, nextRotBounds, true, true);
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
	SetPosition(VMath::VectorConstantLerp(GetPositionV(), nextPos, deltaTime, 12.5f));
	SetRotation(VMath::QuatConstantLerp(GetRotationV(), nextRot, deltaTime, 12.5f));

	HitResult hit(this);
	if (!Raycast(hit, GetPositionV(), -VMath::GlobalUpVector(), 0.5f))
	{
		inFall = true;
		constexpr float fallSpeed = 3.5f;
		nextPos -= VMath::GlobalUpVector() * fallSpeed * deltaTime;
	}
	else if (inFall)
	{
		inFall = false;
		HitResult nodeHit;
		GetCurrentNode()->RecalcNodeHeight(nodeHit);
		nextPos.m128_f32[1] = std::round(nextPos.m128_f32[1]);
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
	nextPos = XMLoadFloat3(&nextNode->worldPosition);

	//When a GridActor moves, need to recalculate the Grid node height of the last position it was at.
	const int lastGridIndexX = (int)std::round(GetPosition().x);
	const int lastGridIndexY = (int)std::round(GetPosition().z);
	GridNode* lastNode = grid->GetNode(lastGridIndexX, lastGridIndexY);
	HitResult lastPosHit(this);
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

bool GridActor::HaveMovementAndRotationStopped()
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
