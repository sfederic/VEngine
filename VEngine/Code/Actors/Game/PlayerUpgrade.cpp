#include "vpch.h"
#include "PlayerUpgrade.h"
#include "Components/MeshComponent.h"

PlayerUpgrade::PlayerUpgrade()
{
	_upgradeMesh = CreateComponent<MeshComponent>("Mesh");
	_upgradeMesh->SetMeshFilename("cube.vmesh");
	SetRootComponent(_upgradeMesh);
}
