#include "vpch.h"
#include "PlayerCameraTrigger.h"
#include "Components/BoxTriggerComponent.h"
#include "Actors/Game/Player.h"

PlayerCameraTrigger::PlayerCameraTrigger()
{
	boxTrigger = CreateComponent<BoxTriggerComponent>("BoxTrigger");
	rootComponent = boxTrigger;
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
			auto player = Player::system.GetOnlyActor();
			player->SetNextCameraPosition(DirectX::XMLoadFloat3(&newLocalCameraPos));
		});
	boxTrigger->SetTriggerExitCallback([&]()
		{
			auto player = Player::system.GetOnlyActor();
			player->SetNextCameraPosition(initialLocalCameraPos);
		});
}

Properties PlayerCameraTrigger::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("NewLocalCameraPos", &newLocalCameraPos);
	return props;
}
