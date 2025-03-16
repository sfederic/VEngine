
#include "GridActorBasket.h"
#include "Player.h"
#include "Actors/ActorSystemCache.h"

Properties GridActorBasket::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("GridActor System", &gridActorSystemNameToSpawnFrom);
	return props;
}

void GridActorBasket::OnLinkActivate()
{
	__super::OnLinkActivate();

	IActorSystem* lSystem = ActorSystemCache::Get().GetSystem(gridActorSystemNameToSpawnFrom);
	if (lSystem)
	{
		Transform transform;
		transform.Decompose(GetWorldMatrix());
		auto actor = lSystem->SpawnActor(transform);
		actor->Create();
		actor->CreateAllComponents();
		actor->Start();

		auto gridActor = dynamic_cast<GridActor*>(actor);
		assert(gridActor);
		auto player = Player::system.GetOnlyActor();
		player->SetLinkedGridActor(*gridActor);
	}
}
