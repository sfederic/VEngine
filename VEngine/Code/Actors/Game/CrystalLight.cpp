#include "vpch.h"
#include "CrystalLight.h"
#include "Components/Lights/PointLightComponent.h"
#include "Components/MeshComponent.h"
#include "Particle/SpriteSheet.h"
#include "Core/VMath.h"

CrystalLight::CrystalLight()
{
	pointLight = CreateComponent<PointLightComponent>("PointLight");
	rootComponent = pointLight;

	mesh = CreateComponent<MeshComponent>("Mesh");
	rootComponent->AddChild(mesh);

	spriteSheet = CreateComponent<SpriteSheet>("Sprite");
	rootComponent->AddChild(spriteSheet);
}

Properties CrystalLight::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}

void CrystalLight::Create()
{
	__super::Create();

	spriteSheet->SetupSpriteData("Sprites/blue_explosion.png", 5, 5, true);
	spriteSheet->SetLocalScale(0.35f);
	spriteSheet->SetAnimationSpeed(15.f);

	pointLight->SetLightColour(XMFLOAT4(0.8f, 0.8f, 1.f, 1.f));

	mesh->SetMeshFilename("crystal.vmesh");
}

void CrystalLight::Start()
{
	lightPulseCounter = VMath::RandomRange(0.f, 1.f);
}

void CrystalLight::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	lightPulseCounter += deltaTime;
	lightPulseIntensity = sinf(lightPulseCounter) + 1.f;
	pointLight->SetIntensity(lightPulseIntensity * 10.f);
}
