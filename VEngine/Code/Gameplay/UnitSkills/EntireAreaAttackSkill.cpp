#include "vpch.h"
#include "EntireAreaAttackSkill.h"
#include "Actors/Game/Grid.h"

EntireAreaAttackSkill::EntireAreaAttackSkill()
{
    auto grid = Grid::system.GetFirstActor();
    range = grid->sizeX * grid->sizeY;
}

std::vector<SkillNode*> EntireAreaAttackSkill::SetNodesForSkillRange(GridNode* startingNode, GridNode* targetNode)
{
    auto grid = Grid::system.GetFirstActor();
    auto nodes = grid->GetAllNodes();
    return SpawnSkillNodes(nodes);
}
