#include "IntuitionComponent.h"

IntuitionComponent::IntuitionComponent()
{
}

Properties IntuitionComponent::GetProps()
{
	Properties props("IntuitionComponent");
	props.Add("Name", &intuitionName);
	props.Add("Description", &intuitionDescription);
	props.Add("Condition", &condition);
	props.Add("Add On Interact", &addOnInteract);
	return props;
}
