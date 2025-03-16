
#include "BloomSeed.h"
import Components.MeshComponent;

BloomSeed::BloomSeed()
{
	flowerBloomMesh = CreateComponent<MeshComponent>("FlowerMesh");
	AddChildToRoot(flowerBloomMesh);
}

void BloomSeed::Create()
{
	__super::Create();

	SetMeshFilename("small_ico_sphere.vmesh");

	flowerBloomMesh->SetMeshFilename("node.vmesh");
	flowerBloomMesh->SetTexture("flower_bloom.png");
	flowerBloomMesh->SetVisibility(false);
	flowerBloomMesh->AddLocalPosition(XMVectorSet(0.f, 0.5f, 0.f, 1.f));
}

void BloomSeed::Start()
{
	__super::Start();

	if (!isPlanted)
	{
		flowerBloomMesh->SetWorldScale(0.f);
	}
}

void BloomSeed::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	if (isPlanted)
	{
		auto scale = flowerBloomMesh->GetWorldScale();
		if (scale.x < 1.f)
		{
			scale.x += 0.3f * deltaTime;
			scale.y += 0.3f * deltaTime;
			scale.z += 0.3f * deltaTime;
			flowerBloomMesh->SetWorldScale(scale);
		}
	}
}

Properties BloomSeed::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("Planted", &isPlanted);
	return props;
}

void BloomSeed::Plant()
{
	if (!isPlanted)
	{
		isPlanted = true;

		flowerBloomMesh->SetVisibility(true);
	}
}
