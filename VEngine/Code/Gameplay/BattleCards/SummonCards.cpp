#include "vpch.h"
#include "BattleCard.h"
#include "BattleCardSystem.h"
#include "Actors/Game/Player.h"

#include "Actors/Game/AllyUnits/AttackUnit.h"
#include "Actors/Game/AllyUnits/SurroundAttackUnit.h"
#include "Actors/Game/AllyUnits/ShootAttackUnit.h"

struct AttackUnitSummonCard : BattleCard
{
	AttackUnitSummonCard() : BattleCard(L"AttackUnitSummonCard", L"Summon an AttackUnit", "UI/monster_icon.jpg")
	{
		cost = 1;
		BattleCardSystem::Get().AddCard(this);
	}

	virtual void Activate() override
	{
		__super::Activate();
		Player::system.GetFirstActor()->SummonAllyUnit<AttackUnit>();
	}
}attackUnitSummonCard;

struct SurroundAttackUnitSummonCard : BattleCard
{
	SurroundAttackUnitSummonCard() : BattleCard(L"SurroundAttackUnitSummonCard", L"Summon an SurroundAttackUnit", "UI/rock.png")
	{
		cost = 1;
		BattleCardSystem::Get().AddCard(this);
	}

	virtual void Activate() override
	{
		__super::Activate();
		Player::system.GetFirstActor()->SummonAllyUnit<SurroundAttackUnit>();
	}
}surroundAttackUnitSummonCard;

struct ShootAttackUnitSummonCard : BattleCard
{
	ShootAttackUnitSummonCard() : BattleCard(L"SurroundAttackUnitSummonCard", L"Summon an ShootAttackUnit", "UI/gun.png")
	{
		cost = 1;
		BattleCardSystem::Get().AddCard(this);
	}

	virtual void Activate() override
	{
		__super::Activate();
		Player::system.GetFirstActor()->SummonAllyUnit<ShootAttackUnit>();
	}
}shootAttackUnitSummonCard;
