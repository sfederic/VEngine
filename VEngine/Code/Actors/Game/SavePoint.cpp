#include "vpch.h"
#include "SavePoint.h"
#include "Components/BoxTriggerComponent.h"
#include "Core/Input.h"
#include "Gameplay/GameUtils.h"
#include "Gameplay/GameInstance.h"
#include "UI/UISystem.h"
#include "UI/Game/PopupWidget.h"

SavePoint::SavePoint()
{
	trigger = CreateComponent<BoxTriggerComponent>("Trigger");
	SetRootComponent(trigger);
}

void SavePoint::Start()
{
	__super::Start();

	trigger->SetTargetAsPlayer();

	savePopupWidget = UISystem::CreateWidget<PopupWidget>();
	savePopupWidget->SetPopupText(L"Save?");
}

void SavePoint::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	if (trigger->ContainsTarget())
	{
		savePopupWidget->AddToViewport();

		if (Input::GetKeyUp(Keys::Enter))
		{
			GameInstance::SetContinueMapName(World::worldFilename);
			GameUtils::SaveGameInstanceData();
			GameUtils::SaveGameWorldState();
		}
	}
	else
	{
		savePopupWidget->RemoveFromViewport();
	}
}

Properties SavePoint::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}
