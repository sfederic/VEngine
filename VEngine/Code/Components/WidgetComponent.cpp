#include "vpch.h"
#include "WidgetComponent.h"
#include "UI/Widget.h"

void WidgetComponent::Destroy()
{
	__super::Destroy();

	if (widget)
	{
		widget->Destroy();
	}
}

Properties WidgetComponent::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}

void WidgetComponent::AddToViewport()
{
	if (widget)
	{
		widget->AddToViewport();
	}
}

void WidgetComponent::RemoveFromViewport()
{
	if (widget)
	{
		widget->RemoveFromViewport();
	}
}

void WidgetComponent::SetPosition(DirectX::XMVECTOR newPosition)
{
	if (widget)
	{
		widget->SetWorldPosition(newPosition);
	}
}
