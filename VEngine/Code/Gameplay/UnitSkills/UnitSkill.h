#pragma once

#include <vector>

struct GridNode;
class SkillNode;

//A skill with a grid range and effect a unit in level can perform.
class UnitSkill
{
protected:
	enum class SkillType
	{
		Damage
	};

	int range = 1;
	SkillType skillType = SkillType::Damage;

	std::vector<SkillNode*> SpawnSkillNodes(std::vector<GridNode*>& gridNodes);

	//This function is for getting a x and y offset direction based on the target.
	//e.g. if the player is at [2, 2] on the grid, this will return the x and y needed to 'target' that target.
	void GetOffsetsForTargetQuadrant(GridNode* startingNode, GridNode* targetNode, int& xOffset, int& yOffset);

public:
	//Spawn AOE esque nodes for every grid node in range of this skill.
	virtual std::vector<SkillNode*> SetNodesForSkillRange(GridNode* startingNode, GridNode* targetNode) = 0;

	int GetRange() { return range; }

	bool IsTargetNodeInRange(GridNode* startingNode, GridNode* targetNode);
};
