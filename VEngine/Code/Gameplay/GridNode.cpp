#include "vpch.h"
#include "GridNode.h"
#include "Actors/Game/Grid.h"
#include "Actors/Game/Player.h"
#include "Actors/Game/WaterVolume.h"
#include "Components/InstanceMeshComponent.h"
#include "Physics/Raycast.h"
#include "Core/VMath.h"

void GridNode::Hide()
{
	active = false;
	DisplayHide();
}

void GridNode::Show()
{
	active = true;
	DisplayShow();
}

void GridNode::DisplayHide() const
{
	auto grid = Grid::system.GetFirstActor();
	auto& meshInstanceData = grid->GetNodeMeshInstanceData().at(instancedMeshIndex);

	meshInstanceData.world.r[0].m128_f32[0] = 0.f;
	meshInstanceData.world.r[1].m128_f32[1] = 0.f;
	meshInstanceData.world.r[2].m128_f32[2] = 0.f;
}

void GridNode::DisplayShow() const
{
	auto grid = Grid::system.GetFirstActor();
	auto& meshInstanceData = grid->GetNodeMeshInstanceData().at(instancedMeshIndex);

	meshInstanceData.world.r[0].m128_f32[0] = 0.9f;
	meshInstanceData.world.r[1].m128_f32[1] = 0.9f;
	meshInstanceData.world.r[2].m128_f32[2] = 0.9f;
}

//@Todo: there are a lot of options to ignore actors here, and future problems.
void GridNode::RecalcNodeHeight(HitResult& hitResult)
{
	const XMVECTOR origin = XMVectorSet((float)xIndex, 20.f, (float)yIndex, 1.f);
	hitResult.actorsToIgnore.push_back(Grid::system.GetOnlyActor());
	auto waterVolumes = WaterVolume::system.GetActorsAsBaseClass();
	hitResult.AddActorsToIgnore(waterVolumes);
	hitResult.actorsToIgnore.push_back(Player::system.GetOnlyActor());

	if (Raycast(hitResult, origin, -VMath::GlobalUpVector(), 40.f))
	{
		for (auto mesh : hitResult.hitActor->GetComponents<MeshComponent>())
		{
			if (mesh->ignoreGridRaycasts)
			{
				Hide();
				return;
			}
		}

		auto grid = Grid::system.GetFirstActor();
		auto& meshInstanceData = grid->GetNodeMeshInstanceData().at(instancedMeshIndex);

		XMFLOAT3 hitPos = hitResult.hitPos;
		hitPos.y += 0.1f;
		XMVECTOR hitPosVector = XMLoadFloat3(&hitPos);
		hitPosVector.m128_f32[3] = 1.0f;

		//set the y-pos for the node
		worldPosition.y = hitPos.y + 0.4f;

		meshInstanceData.world.r[3] = hitPosVector;
	}
}

void GridNode::SetColour(XMFLOAT4 newColour) const
{
	auto grid = Grid::system.GetFirstActor();
	auto& meshInstanceData = grid->GetNodeMeshInstanceData().at(instancedMeshIndex);

	meshInstanceData.colour = newColour;
}
