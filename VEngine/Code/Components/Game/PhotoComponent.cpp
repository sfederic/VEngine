#include "vpch.h"
#include "PhotoComponent.h"
#include "Salvages/SalvageSystem.h"

Properties PhotoComponent::GetProps()
{
    Properties props = __super::GetProps();
    props.title = "PhotoComponent";
    props.Add("Photo Tag", &photoTag);
    return props;
}

bool PhotoComponent::IsTagPartOfCurrentSalvage()
{
    return SalvageSystem::CurrentSalvageContainsPhotoTag(photoTag);
}
