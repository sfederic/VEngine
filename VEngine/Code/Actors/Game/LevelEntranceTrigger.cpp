#include "vpch.h"
#include "LevelEntranceTrigger.h"
#include "Input.h"
#include "VString.h"
#include "FileSystem.h"
#include "Components/BoxTriggerComponent.h"
#include "Actors/Game/PlayerShip.h"
#include "UI/Game/LevelEntranceWidget.h"

LevelEntranceTrigger::LevelEntranceTrigger()
{
	boxTriggerComponent = CreateComponent(BoxTriggerComponent(), "BoxTrigger");
	rootComponent = boxTriggerComponent;
}

void LevelEntranceTrigger::Start()
{
	boxTriggerComponent->targetActor = PlayerShip::system.GetActors()[0];

	levelEntranceWidget = CreateWidget<LevelEntranceWidget>();
	levelEntranceWidget->levelName = levelName;
}

void LevelEntranceTrigger::Tick(float deltaTime)
{
	if (boxTriggerComponent->ContainsTarget())
	{
		levelEntranceWidget->AddToViewport();

		if (Input::GetKeyDown(Keys::Enter))
		{
			FileSystem::LoadWorld(VString::wstos(levelName));
		}
	}
	else
	{
		levelEntranceWidget->RemoveFromViewport();
	}
}

Properties LevelEntranceTrigger::GetProps()
{
	Properties props = __super::GetProps();
	props.Add("Level Name", &levelName);
	return props;
}
