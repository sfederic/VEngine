#pragma once

class Enemy;

namespace CombatManager
{
	void AddActiveEnemy(Enemy* enemy);
	void ChangeToEnemyTurn();
}
