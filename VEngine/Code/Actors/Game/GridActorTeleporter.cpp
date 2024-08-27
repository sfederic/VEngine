#include "vpch.h"
#include "GridActorTeleporter.h"
#include "Components/BoxTriggerComponent.h"
#include "Components/MeshComponent.h"
#include "Actors/Game/GridActor.h"
#include "Gameplay/GameUtils.h"
#include "Actors/Game/Player.h"

GridActorTeleporter::GridActorTeleporter()
{
	boxTrigger = CreateComponent<BoxTriggerComponent>("BoxTrigger");
	SetRootComponent(boxTrigger);

	_teleporterMesh = CreateComponent<MeshComponent>("Mesh");
	AddChildToRoot(_teleporterMesh);
}

void GridActorTeleporter::Create()
{
	__super::Create();

	_teleporterMesh->SetMeshFilename("node.vmesh");
	_teleporterMesh->SetTexture("UI/spellbinding_circle.png");
	_teleporterMesh->SetLocalPosition(0.f, -0.4f, 0.f);
}

void GridActorTeleporter::Start()
{
	__super::Start();

	_otherTeleporter = World::GetActorByNameAndLogCast<GridActorTeleporter>(_otherTeleporterName);
}

void GridActorTeleporter::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	if (!_teleporterUsed && _otherTeleporter != nullptr)
	{
		for (auto gridActor : boxTrigger->GetAllContainedActors<GridActor>())
		{
			for (auto otherTeleporterGridActor : _otherTeleporter->boxTrigger->GetAllContainedActors<GridActor>())
			{
				if (!otherTeleporterGridActor->CheckMovementAndRotationStopped() ||
					!gridActor->CheckMovementAndRotationStopped())
				{
					return;
				}

				//Swap both positions
				const auto gridActorPos = gridActor->GetPositionV();
				const auto otherGridActorPos = otherTeleporterGridActor->GetPositionV();

				gridActor->SetPosition(otherGridActorPos);
				gridActor->SetNextPos(otherGridActorPos);

				otherTeleporterGridActor->SetPosition(gridActorPos);
				otherTeleporterGridActor->SetNextPos(gridActorPos);

				_teleporterUsed = true;

				GameUtils::SpawnSpriteSheet("Sprites/blue_explosion.png", gridActorPos, false, 5, 5);
				GameUtils::SpawnSpriteSheet("Sprites/blue_explosion.png", otherGridActorPos, false, 5, 5);

				Player::system.GetOnlyActor()->ResetLinkedGridActor();

				//Only want to iterate on the first two actors, so return early.
				return;
			}
		}
	}
}

Properties GridActorTeleporter::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("Linked Teleporter", &_otherTeleporterName);
	return props;
}
