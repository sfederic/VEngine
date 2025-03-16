
#include "PowerCrystalLampPost.h"
#include "Components/Lights/PointLightComponent.h"

PowerCrystalLampPost::PowerCrystalLampPost()
{
	pointLight = CreateComponent<PointLightComponent>("PointLight");
	AddChildToRoot(pointLight);
}

void PowerCrystalLampPost::Create()
{
	__super::Create();

	canBeMovedInLink = false;
	canBeRotatedInLink = false;

	SetMeshFilename("lamp_post02.vmesh");

	pointLight->SetLocalPosition(0.f, 2.f, 0.f);
	pointLight->SetActive(false);
}

void PowerCrystalLampPost::PowerUp()
{
	__super::PowerUp();
	pointLight->SetActive(true);
}

void PowerCrystalLampPost::PowerDown()
{
	__super::PowerDown();
	pointLight->SetActive(false);
}
