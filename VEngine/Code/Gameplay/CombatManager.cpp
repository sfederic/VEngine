#include "vpch.h"
#include "CombatManager.h"

std::vector<Enemy*> activeEnemies;

void CombatManager::AddActiveEnemy(Enemy* enemy)
{
	assert(enemy);
	activeEnemies.push_back(enemy);
}

void CombatManager::ChangeToEnemyTurn()
{
}
