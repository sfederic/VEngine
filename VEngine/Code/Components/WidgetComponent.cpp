#include "vpch.h"
#include "WidgetComponent.h"
#include "UI/Widget.h"

void WidgetComponent::Tick(float deltaTime)
{
}

void WidgetComponent::Start()
{
}

void WidgetComponent::Create()
{
}

void WidgetComponent::Destroy()
{
    widget->Destroy();
}

Properties WidgetComponent::GetProps()
{
    return __super::GetProps();
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
        widget->worldPosition = newPosition;
    }
}
