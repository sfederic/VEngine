#pragma once

#include <DirectXMath.h>
#include <DirectXCollision.h>
#include "Physics/CollisionLayers.h"
#include "HitResult.h"

bool Raycast(HitResult& hitResult, DirectX::XMVECTOR origin, DirectX::XMVECTOR direction, float range, bool fromScreen = false);
bool Raycast(HitResult& hitResult, DirectX::XMVECTOR origin, DirectX::XMVECTOR end);
bool RaycastTriangleIntersect(HitResult& hitResult);
bool RaycastFromScreen(HitResult& hitResult);
void DrawDebugLine(DirectX::XMVECTOR start, DirectX::XMVECTOR end);

bool OrientedBoxCast(HitResult& hitResult, DirectX::XMVECTOR origin, DirectX::XMVECTOR end, DirectX::XMFLOAT2 extents, bool drawDebug, bool clearDebugDrawWithTimer);

//Make sure the boundingbox is in world space.
bool OrientedBoxCast(HitResult& hit, DirectX::BoundingOrientedBox& boundsInWorldSpace, bool drawDebug, bool clearDebugDrawWithTimer);

//Doesn't set HitResult::hitActor.
bool SimpleBoxCast(DirectX::XMVECTOR center, DirectX::XMFLOAT3 extents, HitResult& hitResult, bool drawDebug, bool clearDebugDrawWithTimer);
