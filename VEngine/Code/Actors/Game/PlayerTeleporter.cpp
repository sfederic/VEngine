#include "vpch.h"
#include "PlayerTeleporter.h"
#include "Components/BoxTriggerComponent.h"
#include "Player.h"

PlayerTeleporter::PlayerTeleporter()
{
	boxTrigger = CreateComponent<BoxTriggerComponent>("BoxTrigger");
	SetRootComponent(boxTrigger);
}

void PlayerTeleporter::Start()
{
	__super::Start();

	linkedTeleporter = World::GetActorByNameAndLogCast<PlayerTeleporter>(linkedTeleporterName);

	boxTrigger->SetTargetAsPlayer();
}

void PlayerTeleporter::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	if (!hasBeenUsed && boxTrigger->ContainsTarget())
	{
		auto player = Player::system.GetOnlyActor();
		assert(boxTrigger->GetTargetActor() == player);

		player->nextPos = linkedTeleporter->GetPositionV();
		player->SetPosition(linkedTeleporter->GetPositionV());

		linkedTeleporter->hasBeenUsed = true;
		hasBeenUsed = true;
	}
	else if (hasBeenUsed && !boxTrigger->ContainsTarget())
	{
		hasBeenUsed = false;
	}
}

Properties PlayerTeleporter::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("Linked Teleporter", &linkedTeleporterName);
	return props;
}
