#include "vpch.h"
#include "BloomSeed.h"

void BloomSeed::Create()
{
	__super::Create();

	SetMeshFilename("cube.vmesh");
}

void BloomSeed::Tick(float deltaTime)
{
	__super::Tick(deltaTime);
}

Properties BloomSeed::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("Planted", &mIsPlanted).change = std::bind(&BloomSeed::Test, this);
	return props;
}

void BloomSeed::Test()
{
	if (mIsPlanted)
	{
		SetScale(XMVectorSet(1.f, 2.f, 1.f, 1.f));
		RecalcCurrentNodeDontIgnoreThis();
	}
}
