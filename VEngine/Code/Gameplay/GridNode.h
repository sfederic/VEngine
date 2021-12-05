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

		worldPosition = XMFLOAT3((float)x, (float)0.f, (float)y);
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

	void Hide();
	void Show();

	GridNode* parentNode = nullptr;
	XMFLOAT3 worldPosition;

	float gCost = 0.f; //Distance from start node
	float hCost = 0.f; //Distance to end node
	int xIndex = 0;
	int yIndex = 0;
	uint32_t instancedMeshIndex = 0;
	bool closed = false;
	bool active = true;
};
