#include "vpch.h"
#include "BloomSeed.h"

void BloomSeed::Create()
{
	__super::Create();

	SetMeshFilename("cube.vmesh");
}

Properties BloomSeed::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("Planted", &mIsPlanted);
	return props;
}

void BloomSeed::Plant()
{
	if (!mIsPlanted)
	{
		mIsPlanted = true;
		SetScale(XMVectorSet(1.f, 2.f, 1.f, 1.f));
		RecalcCurrentNodeDontIgnoreThis();
	}
}
