#include "PushableGridActor.h"
#include "Gameplay/GameUtils.h"
#include "Player.h"
#include "Physics/Raycast.h"
#include "VMath.h"
#include "Gameplay/GridNode.h"

PushableGridActor::PushableGridActor()
{
}

void PushableGridActor::Start()
{
    __super::Start();

    nextPushMovePos = GetPositionVector();
}

void PushableGridActor::Tick(float deltaTime)
{
    __super::Tick(deltaTime);

    if (isMoving)
    {
        //Keep and eye on this call, the wrong CurrentNode can be gotten if x/y index is too early
        SetGridPosition();

        SetPosition(VMath::VectorConstantLerp(GetPositionVector(), nextPushMovePos, deltaTime, pushMoveSpeed));

        if (XMVector4Equal(nextPushMovePos, GetPositionVector()))
        {
            isMoving = false;
            GetCurrentNode()->Hide();
            GameUtils::GetPlayer()->inInteraction = false;
        }
    }
}

void PushableGridActor::Interact()
{
    if (isMoving)
    {
        return;
    }

    auto player = GameUtils::GetPlayer();

    //The direction to push this actor
    auto playerMeshForward = player->GetMeshForward();

    Ray ray(this);
    ray.actorsToIgnore.push_back(player);
    XMVECTOR endPosition = GetPositionVector() + playerMeshForward;
    if (!Raycast(ray, GetPositionVector(), endPosition)) //If there's nothing in the way...
    {
        nextPushMovePos = endPosition;
        isMoving = true;

        GetCurrentNode()->Show();

        //Make sure player can't move while this actor is moving
        player->inInteraction = true;
    }
}

Properties PushableGridActor::GetProps()
{
    auto props = __super::GetProps();
    props.title = "PushableGridActor";
    props.AddProp(pushMoveSpeed);
    return props;
}
