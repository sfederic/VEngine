#include "vpch.h"
#include "Gameplay/Trap.h"
#include "Actors/Game/Player.h"
#include "Actors/Game/Unit.h"
#include "Actors/Game/Grid.h"
#include "Gameplay/GridNode.h"

struct DamageTrap : Trap
{
	DamageTrap() : Trap(L"DamageTrapCard", L"Inflicts damage.", "UI/trap_icon.jpg")
	{
		//@Todo: need a TrapCardSystem to register each Trap
	}

	virtual void Activate() override
	{
		auto unit = Grid::system.GetFirstActor()->GetUnitAtNode(connectedNode);
		unit->InflictDamage(1);
	}
}damageTrap;

struct StunTrap : Trap
{
	StunTrap() : Trap(L"StunTrapCard ", L"Ends enemy movement that turn.", "UI/stun_icon.png")
	{
	}

	void Activate() override
	{
		auto unit = Grid::system.GetFirstActor()->GetUnitAtNode(connectedNode);
		unit->SetMovePathIndexToMax();
	}
}stunTrap;