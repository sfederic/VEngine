#pragma once
#include <DirectXMath.h>

using namespace DirectX;

struct GridNode
{
	GridNode()
	{
		xIndex = 0;
		yIndex = 0;
	}

	GridNode(int x, int y, int instancedMeshIndex_)
	{
		xIndex = x;
		yIndex = y;
		instancedMeshIndex = instancedMeshIndex_;

		location = XMVectorSet((float)x, (float)y, 0.f, 1.f);
	}

	bool Equals(int x, int y)
	{
		return (x == xIndex) && (y == yIndex);
	}

	bool Equals(GridNode* node)
	{
		return (node->xIndex == xIndex) && (node->yIndex == yIndex);
	}

	float GetFCost()
	{
		return gCost + hCost;
	}

	void ResetValues()
	{
		gCost = 0.f;
		hCost = 0.f;
		parentNode = nullptr;
		closed = false;
	}

	GridNode* parentNode = nullptr;
	XMVECTOR location;

	float gCost; //Distance from start node
	float hCost; //Distance to end node
	int xIndex;
	int yIndex;
	uint32_t instancedMeshIndex;
	bool active;
	bool closed;
};
