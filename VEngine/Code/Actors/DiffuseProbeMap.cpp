#include "DiffuseProbeMap.h"
#include "Components/EmptyComponent.h"

DiffuseProbeMap::DiffuseProbeMap()
{
	rootComponent = EmptyComponent::system.Add(this);
}

Properties DiffuseProbeMap::GetProps()
{
	auto props = __super::GetProps();
	props.title = "DiffuseProbeMap";
	props.AddProp(sizeX);
	props.AddProp(sizeY);
	props.AddProp(sizeZ);
	return props;
}
