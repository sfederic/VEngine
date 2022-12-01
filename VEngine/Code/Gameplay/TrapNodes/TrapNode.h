#pragma once

//Node that activates a gameplay effect when a Unit walks over it. Linked directly to a GridNode.
class TrapNode
{
public:
	virtual void Activate() = 0;
};
