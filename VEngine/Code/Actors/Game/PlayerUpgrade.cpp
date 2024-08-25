#include "vpch.h"
#include "PlayerUpgrade.h"
#include "Components/MeshComponent.h"

PlayerUpgrade::PlayerUpgrade()
{
	_upgradeMesh = CreateComponent<MeshComponent>("Mesh");
	_upgradeMesh->SetMeshFilename("cube.vmesh");
	SetRootComponent(_upgradeMesh);
}

Properties PlayerUpgrade::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("Upgrade Name", &_upgradeGlobalMappingName);
	return props;
}
