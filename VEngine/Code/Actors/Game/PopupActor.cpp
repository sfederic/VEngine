
#include "PopupActor.h"
#include "UI/UISystem.h"
#include "UI/Game/PopupWidget.h"

PopupActor::PopupActor()
{
	SetEmptyRootComponent();
}

void PopupActor::Start()
{
	__super::Start();

	popupWidget = UISystem::CreateWidget<PopupWidget>();
	popupWidget->SetPopupText(popupText);

	if (displayOnLevelLoad)
	{
		popupWidget->AddToViewport();
	}
}

Properties PopupActor::GetProps()
{
	auto props = __super::GetProps();
	props.Add("Popup Text", &popupText);
	props.Add("Display at Load", &displayOnLevelLoad);
	return props;
}
