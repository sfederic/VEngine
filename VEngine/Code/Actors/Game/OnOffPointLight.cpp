#include "vpch.h"
#include "OnOffPointLight.h"
#include "Components/Lights/PointLightComponent.h"
#include "Components/MeshComponent.h"

OnOffPointLight::OnOffPointLight()
{
	pointLight = CreateComponent<PointLightComponent>("PointLight");
	rootComponent = pointLight;
}

void OnOffPointLight::Create()
{
	__super::Create();

	mesh->SetVisibility(false);
	mesh->SetActive(false);
}

Properties OnOffPointLight::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("Is On", &isOn);
	return props;
}

void OnOffPointLight::Start()
{
	__super::Start();

	pointLight->SetActive(isOn);
}

void OnOffPointLight::Activate()
{
	__super::Activate();

	isOn = true;
	pointLight->SetActive(isOn);
}

void OnOffPointLight::Deactivate()
{
	__super::Deactivate();

	isOn = false;
	pointLight->SetActive(isOn);
}
