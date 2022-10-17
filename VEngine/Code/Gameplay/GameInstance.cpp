#include "vpch.h"
#include "GameInstance.h"
#include "UI/Game/GuiltWidget.h"

void GameInstance::DeletePlayerMemories()
{
	playerMemories.clear();
}

void GameInstance::AddGuilt()
{
	//@Todo: have to add guilt back in
	//uiSystem.guiltWidget->AddGuilt();
	//uiSystem.guiltWidget->AddToViewport();
	//CreateWidget<GuiltPopup>()->AddToViewport(4.5f);
}

Properties GameInstance::GetInstanceSaveData()
{
	Properties props("GameInstance");
	props.AddProp(mapToLoadOnContinue);
	return props;
}
