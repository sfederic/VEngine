#include "vpch.h"
#include "InputTutorialGridActor.h"
#include "UI/UISystem.h"
#include "UI/Game/InputIconWidget.h"

void InputTutorialGridActor::Start()
{
	__super::Start();

	inputIconWidget = UISystem::CreateWidget<InputIconWidget>();
}

void InputTutorialGridActor::OnPlayerLinkHover()
{
	inputIconWidget->AddToViewport();
}

void InputTutorialGridActor::OnPlayerLinkHoverOff()
{
	inputIconWidget->RemoveFromViewport();
}
