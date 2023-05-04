#include "vpch.h"
#include "Raycast.h"
#include <limits>
#include "Core/Camera.h"
#include "Render/Renderer.h"
#include "Render/Line.h"
#include "Editor/Editor.h"
#include "Core/VMath.h"
#include "Core/Core.h"
#include "Actors/Actor.h"
#include "Components/MeshComponent.h"
#include "Components/BoxTriggerComponent.h"
#include "Components/Lights/PointLightComponent.h"
#include "Components/Lights/SpotLightComponent.h"
#include "Components/Lights/DirectionalLightComponent.h"
#include "Core/World.h"

bool IsIgnoredActor(Actor* actor, HitResult& hitResult)
{
	for (auto actorToIgnore : hitResult.actorsToIgnore)
	{
		if (actor == actorToIgnore)
		{
			return true;
		}
	}

	return false;
}

bool IsIgnoredSpatialComponent(SpatialComponent* component, HitResult& hitResult)
{
	for (auto componentToIgnore : hitResult.componentsToIgnore)
	{
		if (component == componentToIgnore)
		{
			return true;
		}
	}

	return false;
}

bool Raycast(HitResult& hitResult, XMVECTOR origin, XMVECTOR direction, float range, bool fromScreen)
{
	hitResult.origin = origin;
	hitResult.direction = direction;

	//Calculate raycast from camera coords into world
	if (fromScreen)
	{
		XMVECTOR invViewDet = XMMatrixDeterminant(activeCamera->GetViewMatrix());
		XMMATRIX invView = XMMatrixInverse(&invViewDet, activeCamera->GetViewMatrix());
		XMVECTOR invDet = XMMatrixDeterminant(XMMatrixIdentity());
		XMMATRIX invModel = XMMatrixInverse(&invDet, XMMatrixIdentity());
		XMMATRIX toLocal = XMMatrixMultiply(invView, invModel);

		hitResult.origin = XMVector3TransformCoord(hitResult.origin, toLocal);
		hitResult.direction = XMVector3TransformNormal(hitResult.direction, toLocal);
		hitResult.direction = XMVector3Normalize(hitResult.direction);
	}

	std::vector<float> distances;

	bool bRayHit = false;
	hitResult.hitActors.clear();
	hitResult.hitComponents.clear();

	const auto checkSpatialComponentCollision = [&](SpatialComponent* spatialComponent)
	{
		if (!spatialComponent->IsActive())
		{
			return;
		}

		if (spatialComponent->GetCollisionLayer() == CollisionLayers::None ||
			spatialComponent->GetCollisionLayer() == hitResult.ignoreLayer)
		{
			return;
		}

		if (IsIgnoredSpatialComponent(spatialComponent, hitResult))
		{
			return;
		}

		const BoundingOrientedBox boundingBox = VMath::GetBoundingBoxInWorld(spatialComponent);

		float hitDistance = 0.f;
		if (boundingBox.Intersects(hitResult.origin, hitResult.direction, hitDistance))
		{
			distances.emplace_back(hitDistance);
			hitResult.hitComponents.emplace_back(spatialComponent);
			bRayHit = true;
		}
	};

	auto actorsInWorld = World::GetAllActorsInWorld();
	for (auto actor : actorsInWorld)
	{
		if (!actor->IsActive())
		{
			continue;
		}

		if (IsIgnoredActor(actor, hitResult))
		{
			continue;
		}

		for (auto mesh : actor->GetComponentsOfType<MeshComponent>())
		{
			checkSpatialComponentCollision(mesh);
		}
	}

	//Only add editor mesh components in when gameplay off
	if (!Core::gameplayOn)
	{
		for (auto& pointLight : PointLightComponent::system.GetComponents()) {
			checkSpatialComponentCollision(pointLight.get());
		}
		for (auto& spotLight : SpotLightComponent::system.GetComponents()) {
			checkSpatialComponentCollision(spotLight.get());
		}
		for (auto& directionalLight : DirectionalLightComponent::system.GetComponents()) {
			checkSpatialComponentCollision(directionalLight.get());
		}
		for (auto& boxTrigger : BoxTriggerComponent::system.GetComponents()) {
			checkSpatialComponentCollision(boxTrigger.get());
		}
	}

	float nearestDistance = std::numeric_limits<float>::max();

	if (!bRayHit)
	{
		return false;
	}

	if (RaycastTriangleIntersect(hitResult))
	{
		for (int i = 0; i < distances.size(); i++)
		{
			//You can have negative ranges at some points, like when starting a cast from inside
			//a mesh and hitting the backface of a triangle.
			if (distances[i] > range)
			{
				continue;
			}

			if (distances[i] < nearestDistance)
			{
				nearestDistance = distances[i];
			}
		}
	}

	if (nearestDistance > range)
	{
		return false;
	}

	if (hitResult.hitActor)
	{
		const XMVECTOR hitPos = hitResult.origin + (hitResult.direction * nearestDistance);
		XMStoreFloat3(&hitResult.hitPos, hitPos);

		hitResult.hitDistance = nearestDistance;

		return true;
	}

	return false;
}

bool Raycast(HitResult& hitResult, XMVECTOR origin, XMVECTOR end)
{
	XMVECTOR direction = XMVector3Normalize(end - origin);
	float range = XMVector3Length(end - origin).m128_f32[0] + 0.1f;
	return Raycast(hitResult, origin, direction, range, false);
}

bool RaycastTriangleIntersect(HitResult& hitResult)
{
	std::vector<HitResult> hitResults;

	const auto checkMeshVerticesCollision = [&](MeshComponent& mesh)
	{
		const XMMATRIX meshWorldMatrix = mesh.GetWorldMatrix();

		const auto& vertices = mesh.meshDataProxy.GetVertices();
		const int vertexTriangleCount = vertices.size() / 3;
		for (int i = 0; i < vertexTriangleCount; i++)
		{
			const int index0 = i * 3;
			const int index1 = i * 3 + 1;
			const int index2 = i * 3 + 2;

			XMVECTOR v0 = XMLoadFloat3(&vertices[index0].pos);
			v0 = XMVector3TransformCoord(v0, meshWorldMatrix);

			XMVECTOR v1 = XMLoadFloat3(&vertices[index1].pos);
			v1 = XMVector3TransformCoord(v1, meshWorldMatrix);

			XMVECTOR v2 = XMLoadFloat3(&vertices[index2].pos);
			v2 = XMVector3TransformCoord(v2, meshWorldMatrix);

			float hitDistance = 0.f;
			if (DirectX::TriangleTests::Intersects(hitResult.origin, hitResult.direction, v0, v1, v2, hitDistance))
			{
				HitResult tempHitResult = hitResult;
				tempHitResult.hitDistance = hitDistance;

				//Get normal for triangle
				XMVECTOR normal = XMLoadFloat3(&mesh.meshDataProxy.vertices.at(index0).normal);
				normal = XMVector3TransformNormal(normal, meshWorldMatrix);
				normal = XMVector3Normalize(normal);
				XMStoreFloat3(&tempHitResult.hitNormal, normal);

				//Check if back facing triangle
				const float angleBetweenRaycastDirectionAndTriangleNormal =
					XMConvertToDegrees(XMVector3AngleBetweenNormals(
						normal,
						hitResult.direction).m128_f32[0]);
				if (angleBetweenRaycastDirectionAndTriangleNormal < 90)
				{
					//has hit the back face of a triangle, so skip
					continue;
				}

				//hit position
				const XMVECTOR hitPosition = hitResult.origin + (hitResult.direction * tempHitResult.hitDistance);

				//Hit vertex indices
				std::map<int, XMVECTOR> indexToVertMap;
				indexToVertMap.emplace(index0, v0);
				indexToVertMap.emplace(index1, v1);
				indexToVertMap.emplace(index2, v2);
				tempHitResult.hitVertIndexes.push_back(VMath::GetIndexOfClosestVertexFromTriangleIntersect(indexToVertMap, hitPosition));

				//Get hit UV
				float hitU, hitV;
				VMath::TriangleXYZToUV(mesh.meshDataProxy.vertices.at(index0),
					mesh.meshDataProxy.vertices.at(index1),
					mesh.meshDataProxy.vertices.at(index2), hitPosition, hitU, hitV);
				tempHitResult.uv = XMFLOAT2(hitU, hitV);

				//Set hit component and actor
				tempHitResult.hitComponent = &mesh;
				tempHitResult.hitActor = World::GetActorByUID(mesh.GetOwnerUID());

				hitResults.emplace_back(tempHitResult);
			}
		}
	};

	const auto setDebugMesh = [&](SpatialComponent* component, std::string_view debugMeshName)
	{
		auto debugMesh = MeshComponent::GetDebugMesh("DebugIcoSphere");
		debugMesh->transform = component->transform;
		debugMesh->SetOwnerUID(component->GetOwnerUID());
		checkMeshVerticesCollision(*debugMesh);
	};

	for (auto component : hitResult.hitComponents)
	{
		auto mesh = dynamic_cast<MeshComponent*>(component);
		if (mesh)
		{
			checkMeshVerticesCollision(*mesh);
			continue;
		}

		if (!Core::gameplayOn)
		{
			auto pointLight = dynamic_cast<PointLightComponent*>(component);
			if (pointLight)
			{
				setDebugMesh(pointLight, "DebugIcoSphere");
				continue;
			}

			auto spotLight = dynamic_cast<SpotLightComponent*>(component);
			if (spotLight)
			{
				setDebugMesh(spotLight, "DebugCone");
				continue;
			}

			auto directionalLight = dynamic_cast<DirectionalLightComponent*>(component);
			if (directionalLight)
			{
				setDebugMesh(directionalLight, "DebugSphere");
				continue;
			}

			auto boxTrigger = dynamic_cast<BoxTriggerComponent*>(component);
			if (boxTrigger)
			{
				setDebugMesh(boxTrigger, "DebugBox");
				continue;
			}
		}
	}

	//Get all hit actors
	std::vector<Actor*> hitActors;
	for (auto& hitResult : hitResults)
	{
		hitActors.emplace_back(hitResult.hitActor);
	}

	//Set nearest hit actor
	float lowestDistance = std::numeric_limits<float>::max();
	int rayIndex = -1;
	for (int i = 0; i < hitResults.size(); i++)
	{
		if (hitResults[i].hitDistance < lowestDistance)
		{
			lowestDistance = hitResults[i].hitDistance;
			rayIndex = i;
		}
	}

	if (rayIndex > -1)
	{
		hitResult = hitResults[rayIndex];
		hitResult.hitActors = hitActors;
		return true;
	}

	return false;
}

bool RaycastFromScreen(HitResult& hitResult)
{
	const int sx = editor->viewportMouseX;
	const int sy = editor->viewportMouseY;

	CameraComponent* camera = activeCamera;

	XMMATRIX proj = camera->GetProjectionMatrix();

	const float vx = (2.f * sx / Renderer::GetViewportWidth() - 1.0f) / proj.r[0].m128_f32[0];
	const float vy = (-2.f * sy / Renderer::GetViewportHeight() + 1.0f) / proj.r[1].m128_f32[1];

	hitResult.origin = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	hitResult.direction = XMVectorSet(vx, vy, 1.f, 0.f);

	constexpr float range = std::numeric_limits<float>::max();

	return Raycast(hitResult, hitResult.origin, hitResult.direction, range, true);
}

void DrawDebugLine(XMVECTOR start, XMVECTOR end)
{
	Line line;
	XMStoreFloat3(&line.p1, start);
	XMStoreFloat3(&line.p2, end);

	Renderer::AddDebugLine(line);
}

//@Todo: can consolidate the two orientedboxcast functions here
bool OrientedBoxCast(HitResult& hit, BoundingOrientedBox& bb, bool drawDebug, bool clearDebugDrawWithTimer)
{
	if (drawDebug)
	{
		Renderer::AddDebugDrawOrientedBox(bb, clearDebugDrawWithTimer);
	}

	for (auto actor : World::GetAllActorsInWorld())
	{
		if (IsIgnoredActor(actor, hit))
		{
			continue;
		}

		for (auto mesh : actor->GetComponentsOfType<MeshComponent>())
		{
			auto meshBoundsInWorld = VMath::GetBoundingBoxInWorld(mesh);
			if (bb.Intersects(meshBoundsInWorld))
			{
				hit.hitComponents.emplace_back(mesh);
				hit.hitActors.emplace_back(actor);
			}
		}
	}

	return hit.hitActors.size();
}

bool OrientedBoxCast(HitResult& hitResult, XMVECTOR origin, XMVECTOR end, XMFLOAT2 extents, bool drawDebug, bool clearDebugDrawWithTimer)
{
	XMVECTOR orientationV = VMath::LookAtRotation(end, origin);

	XMFLOAT4 orientation{};
	XMStoreFloat4(&orientation, orientationV);

	XMVECTOR centerV = VMath::CalcMidPoint(origin, end);
	XMFLOAT3 center{};
	XMStoreFloat3(&center, centerV);

	XMFLOAT3 extentsFloat3 = { extents.x, extents.y, 0.f };
	extentsFloat3.z = XMVector3Length(end - origin).m128_f32[0] / 2.f;

	DirectX::BoundingOrientedBox boundingOrientedBox(center, extentsFloat3, orientation);

	if (drawDebug)
	{
		Renderer::AddDebugDrawOrientedBox(boundingOrientedBox, clearDebugDrawWithTimer);
	}

	for (auto actor : World::GetAllActorsInWorld())
	{
		if (IsIgnoredActor(actor, hitResult))
		{
			continue;
		}

		for (auto mesh : actor->GetComponentsOfType<MeshComponent>())
		{
			auto meshBoundsInWorld = VMath::GetBoundingBoxInWorld(mesh);
			if (boundingOrientedBox.Intersects(meshBoundsInWorld))
			{
				hitResult.hitComponents.emplace_back(mesh);
				hitResult.hitActors.emplace_back(actor);
			}
		}
	}

	return hitResult.hitActors.size();
}

//Note: doesn't set HitResult::hitActor.
bool SimpleBoxCast(XMVECTOR center, XMFLOAT3 extents, HitResult& hit, bool drawDebug, bool clearDebugDrawWithTimer)
{
	XMFLOAT3 centerFloat3;
	XMStoreFloat3(&centerFloat3, center);
	DirectX::BoundingBox boundingBox(centerFloat3, extents);

	if (drawDebug)
	{
		DirectX::BoundingOrientedBox orientedBox;
		DirectX::BoundingOrientedBox::CreateFromBoundingBox(orientedBox, boundingBox);
		Renderer::AddDebugDrawOrientedBox(orientedBox, clearDebugDrawWithTimer);
	}

	for (auto actor : World::GetAllActorsInWorld())
	{
		if (IsIgnoredActor(actor, hit))
		{
			continue;
		}

		for (auto mesh : actor->GetComponentsOfType<MeshComponent>())
		{
			BoundingOrientedBox meshWorldBounds = VMath::GetBoundingBoxInWorld(mesh);

			if (boundingBox.Intersects(meshWorldBounds))
			{
				hit.hitComponents.emplace_back(mesh);
				hit.hitActors.emplace_back(actor);
			}
		}
	}

	return hit.hitActors.size();
}

void HitResult::AddActorsToIgnore(std::vector<Actor*>& actors)
{
	actorsToIgnore.reserve(actorsToIgnore.size() + actors.size());

	for (auto actor : actors)
	{
		actorsToIgnore.emplace_back(actor);
	}
}
