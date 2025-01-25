#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"

class BoxTriggerComponent;

//Actor that repositions the player's camera and sets new target actors when within bounds. 
//Resets to previous values when player leaves this actor's bounds. 
//(This actor came about from dynamic camera musings from looking at cinematography in Evangelion.)
class PlayerCameraTrigger : public Actor
{
public:
	ACTOR_SYSTEM(PlayerCameraTrigger);

	PlayerCameraTrigger();
	void Start() override;
	void LateStart() override;
	Properties GetProps() override;

	bool ContainsPlayer();
	void ResetCameraPosAndTarget();

private:
	std::string newTargetActorName;

	BoxTriggerComponent* boxTrigger = nullptr;

	DirectX::XMVECTOR initialLocalCameraPos = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);
	DirectX::XMFLOAT3 newLocalCameraPos = DirectX::XMFLOAT3(1.f, 1.f, -1.f);

	Actor* newTargetActor = nullptr;
	Actor* initialTargetActor = nullptr;
};
