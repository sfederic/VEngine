#include "vpch.h"
#include "BulletCache.h"
#include "Player.h"
#include "Gameplay/GameUtils.h"

BulletCache::BulletCache()
{
    interactText = L"Stocked up ammunition.";
    isDestructible = false;
}

Properties BulletCache::GetProps()
{
    auto props = __super::GetProps();
    props.title = "BulletCache";
    return props;
}

void BulletCache::Interact()
{
    Player::system.GetFirstActor()->ResetBulletsToMax();
    GameUtils::PlayAudioOneShot("gun_reload.wav");
}
