#include "vpch.h"
#include "TrapCard.h"
#include "BattleCardSystem.h"
#include "Actors/Game/Player.h"
#include "Actors/Game/Unit.h"
#include "Actors/Game/Grid.h"
#include "Gameplay/GridNode.h"

struct DamageTrapCard : TrapCard
{
	DamageTrapCard() : TrapCard(L"DamageTrapCard", L"Inflicts damage.", "UI/trap_icon.jpg")
	{
		BattleCardSystem::Get().AddCard(this);
	}

	virtual void ActivateTrap() override
	{
		auto unit = Grid::system.GetFirstActor()->GetUnitAtNode(connectedNode);
		unit->InflictDamage(1);
	}
}damageTrapCard;

struct StunTrapCard : TrapCard
{
	StunTrapCard() : TrapCard(L"StunTrapCard ", L"Ends enemy movement that turn.", "UI/stun_icon.png")
	{
		BattleCardSystem::Get().AddCard(this);
	}

	virtual void ActivateTrap() override
	{
		auto unit = Grid::system.GetFirstActor()->GetUnitAtNode(connectedNode);
		unit->SetMovePathIndexToMax();
	}
}stunTrapCard;