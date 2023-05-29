#include "vpch.h"
#include "OilSlick.h"
#include "Components/MeshComponent.h"

void OilSlick::Create()
{
    canBeRotatedInLink = false;
    canBeRotatedInLink = false;

    mesh->SetMeshFilename("node.vmesh");
}

Properties OilSlick::GetProps()
{
    auto props = __super::GetProps();
    props.title = GetTypeName();
    return props;
}
