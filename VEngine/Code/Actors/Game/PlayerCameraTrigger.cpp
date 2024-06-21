#include "vpch.h"
#include "PlayerCameraTrigger.h"
#include "Components/BoxTriggerComponent.h"
#include "Actors/Game/Player.h"

PlayerCameraTrigger::PlayerCameraTrigger()
{
	boxTrigger = CreateComponent<BoxTriggerComponent>("BoxTrigger");
	SetRootComponent(boxTrigger);
}

void PlayerCameraTrigger::Start()
{
	boxTrigger->SetTargetAsPlayer();
	{
		//Set initial camera position outside of trigger.
		const auto player = Player::system.GetOnlyActor();
		initialLocalCameraPos = player->GetCameraLocalPosition();
	}
	boxTrigger->SetTriggerEnterCallback([&]()
		{
			ResetCameraPosAndTarget();
		});
	boxTrigger->SetTriggerExitCallback([&]()
		{
			auto player = Player::system.GetOnlyActor();
			player->SetNextCameraPosition(initialLocalCameraPos);
			player->SetCameraTargetActor(initialTargetActor);
		});
}

void PlayerCameraTrigger::LateStart()
{
	//Set these fields on late start so player has a chance to set up camera
	//and new targets have a chance to spawn.
	newTargetActor = World::GetActorByNameAllowNull(newTargetActorName);
	auto player = Player::system.GetOnlyActor();
	initialTargetActor = &player->GetCameraTargetActor();
	assert(initialTargetActor);
}

Properties PlayerCameraTrigger::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("NewLocalCameraPos", &newLocalCameraPos);
	props.Add("NewTargetActor", &newTargetActorName).useActorsAutoComplete = true;
	return props;
}

bool PlayerCameraTrigger::ContainsPlayer()
{
	return boxTrigger->ContainsTarget();
}

void PlayerCameraTrigger::ResetCameraPosAndTarget()
{
	auto player = Player::system.GetOnlyActor();
	player->SetNextCameraPosition(DirectX::XMLoadFloat3(&newLocalCameraPos));
	if (newTargetActor != nullptr)
	{
		player->SetCameraTargetActor(newTargetActor);
	}
}
