#include "vpch.h"
#include "ReconActor.h"

Properties ReconActor::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("Recon Text", &reconText);
	return props;
}
