#include "vpch.h"
#include "WidgetComponent.h"
#include "UI/Widget.h"

void WidgetComponent::Destroy()
{
	__super::Destroy();

	widget->Destroy();
}

Properties WidgetComponent::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
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
		widget->SetWorldPosition(newPosition);
	}
}
