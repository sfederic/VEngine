#include "vpch.h"
#include "Component.h"
#include "Actors/Actor.h"

Properties Component::GetProps()
{
    Properties props{};
    //props.Add("Name", &name).hide = true;
    //props.Add("OwnerUID", &ownerUID).hide = true;
    return props;
}
