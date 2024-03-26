#include "vpch.h"
#include "InputTutorialGridActor.h"
#include "UI/UISystem.h"
#include "UI/Game/InputIconWidget.h"

static const std::string enterKeyImage = "UI/enter_key_icon.jpg";
static const std::string WASDArrowsKeyImage = "UI/wasd_arrows_icon.jpg";

InputTutorialGridActor::InputTutorialGridActor()
{
	iconImage = enterKeyImage;
}

void InputTutorialGridActor::Start()
{
	__super::Start();

	inputIconWidget = UISystem::CreateWidget<InputIconWidget>();
}

void InputTutorialGridActor::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	inputIconWidget->SetIconImageFilename(iconImage);
}

Properties InputTutorialGridActor::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("Icon Image", &iconImage);
	return props;
}

void InputTutorialGridActor::OnPlayerLinkHover()
{
	inputIconWidget->AddToViewport();
}

void InputTutorialGridActor::OnPlayerLinkHoverOff()
{
	inputIconWidget->RemoveFromViewport();
}

void InputTutorialGridActor::OnLinkActivate()
{
	iconImage = WASDArrowsKeyImage;
	inputIconWidget->AddToViewport();
}

void InputTutorialGridActor::OnLinkDeactivate()
{
	iconImage = enterKeyImage;
}
