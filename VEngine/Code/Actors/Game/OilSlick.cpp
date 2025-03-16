
#include "OilSlick.h"
import Components.MeshComponent;
#include "FireSource.h"

void OilSlick::Create()
{
	__super::Create();

	DisableAllInteractivity();

	_mesh->SetMeshFilename("node.vmesh");
	_mesh->SetTexture("oil_slick.png");
}

Properties OilSlick::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}

void OilSlick::Burn()
{
	__super::Burn();

	if (!onFire)
	{
		Transform t;
		t.Decompose(GetWorldMatrix());

		auto fireSource = FireSource::system.Add(t);
		fireSource->Create();
		fireSource->CreateAllComponents();
		fireSource->Start();

		onFire = true;
	}
}
