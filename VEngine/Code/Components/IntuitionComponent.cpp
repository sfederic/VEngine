#include "IntuitionComponent.h"

IntuitionComponent::IntuitionComponent()
{
}

Properties IntuitionComponent::GetProps()
{
	Properties props("IntuitionComponent");
	props.Add("Name", &intName);
	props.Add("Description", &intDescription);
	props.Add("Add On Interact", &addOnInteract);
	return props;
}
