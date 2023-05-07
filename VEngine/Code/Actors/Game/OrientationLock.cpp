#include "vpch.h"
#include "OrientationLock.h"
#include "Core/VMath.h"
#include "Core/Log.h"

void OrientationLock::Tick(float deltaTime)
{
    __super::Tick(deltaTime);

    if (!orientationActivated && IsOrientationCorrect())
    {
        orientationActivated = true;

        auto actor = World::GetActorByNameAllowNull(gridActorToActivateOnCorrectOrientation);
        if (actor)
        {
            //.. dynamic cast to a 'trigger' actor
        }
        else
        {
            Log("[%s] not found.", gridActorToActivateOnCorrectOrientation.c_str());
        }
    }
}

Properties OrientationLock::GetProps()
{
    auto props = __super::GetProps();
    props.title = GetTypeName();
    props.Add("Direction", &direction);
    props.Add("Linked GridActor", &gridActorToActivateOnCorrectOrientation);
    return props;
}

bool OrientationLock::IsOrientationCorrect()
{
    auto forward = GetForwardVectorV();
    auto dir = XMLoadFloat3(&direction);
    return VMath::VecEqual(forward, dir);
}
