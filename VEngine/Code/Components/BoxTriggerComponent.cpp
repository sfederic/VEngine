#include "BoxTriggerComponent.h"
#include "Components/MeshComponent.h"

BoxTriggerComponent::BoxTriggerComponent()
{
	
}

void BoxTriggerComponent::Tick(double deltaTime)
{

}

Properties BoxTriggerComponent::GetProps()
{
	Properties props("BoxTriggerComponent");
	props.Add("Extents", &boundingBox.Extents);
	return props;
}

void BoxTriggerComponent::Create()
{

}
