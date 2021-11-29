#include "IntuitionComponent.h"

IntuitionComponent::IntuitionComponent()
{
}

Properties IntuitionComponent::GetProps()
{
	Properties props("IntuitionComponent");
	props.Add("Name", &name);
	props.Add("Description", &description);
	return props;
}
