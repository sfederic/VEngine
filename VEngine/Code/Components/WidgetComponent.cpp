#include "WidgetComponent.h"
#include "VString.h"

void WidgetComponent::Tick(double deltaTime)
{
}

void WidgetComponent::Create()
{
}

Properties WidgetComponent::GetProps()
{
    return Properties();
}

void WidgetComponent::SetText(std::string text)
{
    widget.displayText = stows(text);
}

void WidgetComponent::AddToViewport()
{
    widget.AddToViewport();
}

void WidgetComponent::RemoveFromViewport()
{
    widget.RemoveFromViewport();
}
