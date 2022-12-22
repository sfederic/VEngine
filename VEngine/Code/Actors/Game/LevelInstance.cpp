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

	//The '0.1f's here are just a small offset to work with to handle floating point accuracy.
	if (pos.x < -0.1f || pos.x > levelSize.x + 0.1f)
	{
		return false;
	}
	else if (pos.y < -0.1f || pos.y > levelSize.y + 0.1f)
	{
		return false;
	}
	else if (pos.z < -0.1f || pos.z > levelSize.z + 0.1f)
	{
		return false;
	}

    return true;
}
