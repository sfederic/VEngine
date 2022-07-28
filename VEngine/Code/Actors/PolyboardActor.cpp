#include "vpch.h"
#include "PolyboardActor.h"
#include "Particle/Polyboard.h"

PolyboardActor::PolyboardActor()
{
    polyboard = Polyboard::system.Add("PolyBoard", this);
}

Properties PolyboardActor::GetProps()
{
    return __super::GetProps();
}
