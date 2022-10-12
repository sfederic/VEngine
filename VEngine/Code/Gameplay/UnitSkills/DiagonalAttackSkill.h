#pragma once

#include "UnitSkill.h"

//Think like a bishop in chess.
class DiagonalAttackSkill : public UnitSkill
{
public:
	DiagonalAttackSkill();
	virtual std::vector<SkillNode*> SetNodesForSkillRange(GridNode* startingNode, GridNode* targetNode) override;
};
