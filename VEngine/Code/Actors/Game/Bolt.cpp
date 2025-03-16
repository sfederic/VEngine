
#include "Bolt.h"

void Bolt::Create()
{
	__super::Create();

	SetMeshFilename("bolt.vmesh");
}

Properties Bolt::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}
