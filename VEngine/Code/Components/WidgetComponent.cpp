#include "WidgetComponent.h"
#include "UI/Widget.h"
#include "VString.h"

void WidgetComponent::Tick(float deltaTime)
{
}

void WidgetComponent::Start()
{
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
    if (widget)
    {
        widget->pos = newPosition;
    }
}
