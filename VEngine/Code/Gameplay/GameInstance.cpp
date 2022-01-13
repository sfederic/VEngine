#include "GameInstance.h"
#include "UI/UISystem.h"
#include "UI/GuiltWidget.h"

void GameInstance::DeletePlayerMemories()
{
	playerMemories.clear();
}

void GameInstance::ProgressTime()
{
	currentMinute += 15;
	if (currentMinute >= 60)
	{
		currentHour++;
		currentMinute = 0;
	}
}

void GameInstance::ResetTime()
{
	currentHour = 0;
	currentMinute = 0;
}

void GameInstance::AddGuilt()
{
	//uiSystem.guiltWidget->AddGuilt();
	//uiSystem.guiltWidget->AddToViewport();
	//CreateWidget<GuiltPopup>()->AddToViewport(4.5f);
}

Properties GameInstance::GetInstanceSaveData()
{
	Properties props("GameInstance");
	props.AddProp(currentHour);
	props.AddProp(currentMinute);
	props.AddProp(mapToLoadOnContinue);
	return props;
}
