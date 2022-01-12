#include "PushableGridActor.h"
#include "Gameplay/GameUtils.h"
#include "Player.h"
#include "Physics/Raycast.h"

PushableGridActor::PushableGridActor()
{
}

void PushableGridActor::Start()
{
    nextPushMovePos = GetPositionVector();
}

void PushableGridActor::Tick(float deltaTime)
{
    if (isMoving)
    {
        SetPosition(nextPushMovePos);

        if (XMVector4Equal(nextPushMovePos, GetPositionVector()))
        {
            isMoving = false;
        }
    }
}

void PushableGridActor::Interact()
{
    auto player = GameUtils::GetPlayer();

    //The direction to push this actor
    auto playerForward = player->GetForwardVectorV();

    Ray ray(this);
    ray.actorsToIgnore.push_back(player);
    XMVECTOR endPosition = GetPositionVector() + playerForward;
    if (!Raycast(ray, GetPositionVector(), endPosition)) //If there's nothing in the way...
    {
        nextPushMovePos = endPosition;
        isMoving = true;
    }
}

Properties PushableGridActor::GetProps()
{
    auto props = __super::GetProps();
    return props;
}
