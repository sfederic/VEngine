#include "vpch.h"
#include "OilSlick.h"
#include "Components/MeshComponent.h"
#include "Gameplay/GameUtils.h"
#include "FireSource.h"

void OilSlick::Create()
{
    canBeRotatedInLink = false;
    canBeRotatedInLink = false;

    mesh->SetMeshFilename("node.vmesh");
    mesh->SetTexture("oil_slick.png");
}

Properties OilSlick::GetProps()
{
    auto props = __super::GetProps();
    props.title = GetTypeName();
    return props;
}

void OilSlick::Burn()
{
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
