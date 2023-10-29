#include "vpch.h"
#include "TitleScreenActor.h"
#include "UI/TitlescreenWidget.h"
#include "UI/UISystem.h"

TitleScreenActor::TitleScreenActor()
{
	SetEmptyRootComponent();
}

void TitleScreenActor::Start()
{
	__super::Start();

	titleScreenWidget = UISystem::CreateWidget<TitleScreenWidget>();
	titleScreenWidget->AddToViewport();
}

Properties TitleScreenActor::GetProps()
{
	return Properties("TitleScreenActor");
}
