
#include "OnOffPointLight.h"
#include "Components/Lights/PointLightComponent.h"
import Components.MeshComponent;

OnOffPointLight::OnOffPointLight()
{
	pointLight = CreateComponent<PointLightComponent>("PointLight");
	SetRootComponent(pointLight);
}

void OnOffPointLight::Create()
{
	__super::Create();

	_mesh->SetVisibility(false);
	_mesh->SetActive(false);
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
