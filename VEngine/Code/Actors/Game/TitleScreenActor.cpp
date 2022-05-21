#include "vpch.h"
#include "TitleScreenActor.h"
#include "UI/TitlescreenWidget.h"
#include "Components/EmptyComponent.h"

TitleScreenActor::TitleScreenActor()
{
    rootComponent = EmptyComponent::system.Add();
}

void TitleScreenActor::Start()
{
    titlescreenWidget = CreateWidget<TitleScreenWidget>();
    titlescreenWidget->AddToViewport();
}

Properties TitleScreenActor::GetProps()
{
    return Properties();
}
