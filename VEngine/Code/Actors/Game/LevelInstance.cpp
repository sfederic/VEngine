#include "vpch.h"
#include "LevelInstance.h"

LevelInstance::LevelInstance()
{
    SetEmptyRootComponent();
}

Properties LevelInstance::GetProps()
{
    auto props = __super::GetProps();
    props.Add("Level Size", &levelSize);
    return props;
}

bool LevelInstance::CheckIfPointInsideLevelSize(XMVECTOR point)
{
	XMFLOAT3 pos;
	XMStoreFloat3(&pos, point);

	if (pos.x < 0.f || pos.x > levelSize.x)
	{
		return false;
	}
	else if (pos.y < 0.f || pos.y > levelSize.y)
	{
		return false;
	}
	else if (pos.z < 0.f || pos.z > levelSize.z)
	{
		return false;
	}

    return true;
}
