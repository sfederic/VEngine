#pragma once

#include <DirectXMath.h>

using namespace DirectX;

struct HitResult;
struct TrapCard;

struct GridNode
{
	GridNode() {}

	GridNode(int x, int y, int instancedMeshIndex_)
	{
		xIndex = x;
		yIndex = y;
		instancedMeshIndex = instancedMeshIndex_;

		worldPosition = XMFLOAT3((float)x, 0.f, (float)y);
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
		preview = false;
	}

	//These functions also sets the nodes variables
	void Hide();
	void Show();

	//These ones only care about the visual display of the node
	void DisplayHide();
	void DisplayShow();

	//Use a raycast recalculate node's world position height.
	//Has the Ray here because sometimes you might need to add the owning actor to the ignore list for the array.
	void RecalcNodeHeight(HitResult& hit);

	void SetColour(XMFLOAT4 newColour);

	GridNode* parentNode = nullptr;
	XMFLOAT3 worldPosition = XMFLOAT3(0.f, 0.f, 0.f);

	XMVECTOR GetWorldPosV() { return XMLoadFloat3(&worldPosition); }

	//Linked Trap BattleCard attached to this GridNode
	TrapCard* trapCard = nullptr;

	//COLOURS
	inline static XMFLOAT4 normalColour = XMFLOAT4(0.07f, 0.27f, 0.89f, 0.4f);
	inline static XMFLOAT4 previewColour = XMFLOAT4(0.89f, 0.07f, 0.07f, 0.4f);
	inline static XMFLOAT4 trapNodeColour = XMFLOAT4(0.9f, 0.45f, 0.1f, 0.7f);

	float gCost = 0.f; //Distance from start node
	float hCost = 0.f; //Distance to end node
	int xIndex = 0;
	int yIndex = 0;
	uint32_t instancedMeshIndex = 0;
	bool closed = false;
	bool active = true;
	bool preview = false; //If the node is to show preview movements, ignores lerp
};
