#include "WidgetComponent.h"
#include "VString.h"

void WidgetComponent::Tick(float deltaTime)
{
}

void WidgetComponent::Start()
{
    //widget->Start();
}

void WidgetComponent::Create()
{
}

Properties WidgetComponent::GetProps()
{
    return Properties();
}

void WidgetComponent::AddToViewport()
{
    widget->AddToViewport();
}

void WidgetComponent::RemoveFromViewport()
{
    widget->RemoveFromViewport();
}

void WidgetComponent::SetPosition(XMVECTOR newPosition)
{
    widget->pos = newPosition;
}
