#include "vpch.h"
#include "Raycast.h"
#include <limits>
#include "Camera.h"
#include "Render/Renderer.h"
#include "Editor/Editor.h"
#include "VMath.h"
#include "Actors/Actor.h"
#include "Components/MeshComponent.h"
#include "World.h"

//void DrawRayDebug(XMVECTOR rayOrigin, XMVECTOR rayDir, float distance)
//{
//	Line debugLine = {};
//	XMStoreFloat3(&debugLine.p1, rayOrigin);
//	XMVECTOR pos = XMLoadFloat3(&debugLine.p1);
//	pos += activeCamera->GetRightVectorV();
//	XMStoreFloat3(&debugLine.p1, pos);
//
//	if (distance <= 0.f)
//	{
//		distance = 0.f;
//	}
//
//	XMVECTOR dist = rayDir * distance;
//	XMVECTOR rayEnd = rayOrigin + dist;
//	XMStoreFloat3(&debugLine.p2, rayEnd);
//
//	debugLines.push_back(debugLine);
//}

bool Raycast(Ray& ray, XMVECTOR origin, XMVECTOR direction, float range, bool fromScreen)
{
	ray.origin = origin;
	ray.direction = direction;

	//Calculate raycast from camera coords into world
	if (fromScreen)
	{
		XMVECTOR invViewDet = XMMatrixDeterminant(activeCamera->GetViewMatrix());
		XMMATRIX invView = XMMatrixInverse(&invViewDet, activeCamera->GetViewMatrix());
		XMVECTOR invDet = XMMatrixDeterminant(XMMatrixIdentity());
		XMMATRIX invModel = XMMatrixInverse(&invDet, XMMatrixIdentity());
		XMMATRIX toLocal = XMMatrixMultiply(invView, invModel);

		ray.origin = XMVector3TransformCoord(ray.origin, toLocal);
		ray.direction = XMVector3TransformNormal(ray.direction, toLocal);
		ray.direction = XMVector3Normalize(ray.direction);
	}

	std::vector<float> distances;

	bool bRayHit = false;
	ray.hitActors.clear();
	ray.hitComponents.clear();

	for (Actor* actor : world.GetAllActorsInWorld())
	{
		if (!actor->IsActive())
		{
			continue;
		}

		//Skip if actor is in ignore list
		for (Actor* actorToIgnore : ray.actorsToIgnore)
		{
			if (actorToIgnore == actor)
			{
				goto endActorIteration;
			}
		}

		//Iterate over actor's mesh components
		for (auto mesh : actor->GetComponentsOfType<MeshComponent>())
		{
			//Collision layer checks
			if (mesh->layer == CollisionLayers::None ||
				mesh->layer == ray.ignoreLayer)
			{
				continue;
			}

			BoundingOrientedBox boundingBox = VMath::GetUpdatedBoundingBox(mesh);

			if (boundingBox.Intersects(ray.origin, ray.direction, ray.hitDistance))
			{
				distances.push_back(ray.hitDistance);
				ray.hitActors.push_back(actor);
				ray.hitComponents.push_back(mesh);
				bRayHit = true;
			}
		}

		endActorIteration:
		continue;
	}

	float nearestDistance = std::numeric_limits<float>::max();

	//Figure out the distance
	if (!bRayHit)
	{
		return false;
	}

	if (RaycastTriangleIntersect(ray))
	{
		for (int i = 0; i < distances.size(); i++)
		{
			//Skip hit if more than range or less than 0
			if (distances[i] > range || distances[i] < 0.f)
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

	if (ray.hitActor)
	{
		XMVECTOR hitPos = origin + (direction * ray.hitDistance);
		XMStoreFloat3(&ray.hitPos, hitPos);
		return true;
	}

	return false;
}

bool Raycast(Ray& ray, XMVECTOR origin, XMVECTOR end)
{
	XMVECTOR direction = XMVector3Normalize(end - origin);
	float range = XMVector3Length(end - origin).m128_f32[0] + 0.1f;
	return Raycast(ray, origin, direction, range, false);
}

bool RaycastTriangleIntersect(Ray& ray)
{
	std::vector<Ray> rays;

	for (auto component : ray.hitComponents)
	{
		XMMATRIX model = component->GetWorldMatrix();

		auto mesh = dynamic_cast<MeshComponent*>(component);
		if(mesh)
		{
			//This is for dealing with DestructibleMesh's meshproxy nulls
			if (mesh->meshDataProxy->vertices == nullptr || mesh->meshDataProxy->indices == nullptr)
			{
				continue;
			}

			for (int i = 0; i < mesh->meshDataProxy->vertices->size() / 3; i++)
			{
				MeshData::indexDataType index0 = mesh->meshDataProxy->indices->at(static_cast<std::vector<MeshData::indexDataType, std::allocator<MeshData::indexDataType>>::size_type>(i) * 3);
				MeshData::indexDataType index1 = mesh->meshDataProxy->indices->at(static_cast<std::vector<MeshData::indexDataType, std::allocator<MeshData::indexDataType>>::size_type>(i) * 3 + 1);
				MeshData::indexDataType index2 = mesh->meshDataProxy->indices->at(static_cast<std::vector<MeshData::indexDataType, std::allocator<MeshData::indexDataType>>::size_type>(i) * 3 + 2);

				XMVECTOR v0 = XMLoadFloat3(&mesh->meshDataProxy->vertices->at(index0).pos);
				v0 = XMVector3TransformCoord(v0, model);

				XMVECTOR v1 = XMLoadFloat3(&mesh->meshDataProxy->vertices->at(index1).pos);
				v1 = XMVector3TransformCoord(v1, model);

				XMVECTOR v2 = XMLoadFloat3(&mesh->meshDataProxy->vertices->at(index2).pos);
				v2 = XMVector3TransformCoord(v2, model);

				Ray tempRay = {};
				tempRay = ray;
				tempRay.hitActors.clear();

				if (DirectX::TriangleTests::Intersects(ray.origin, ray.direction, v0, v1, v2, tempRay.hitDistance))
				{
					XMVECTOR normal = XMVectorZero();
					normal += XMLoadFloat3(&mesh->meshDataProxy->vertices->at(index0).normal);
					normal += XMLoadFloat3(&mesh->meshDataProxy->vertices->at(index1).normal);
					normal += XMLoadFloat3(&mesh->meshDataProxy->vertices->at(index2).normal);

					normal = XMVector3Normalize(normal);

					XMStoreFloat3(&tempRay.normal, normal);

					tempRay.hitComponent = mesh;

					tempRay.hitActor = world.GetActorByUID(mesh->ownerUID);

					rays.push_back(tempRay);
				}
			}
		}
	}

	float lowestDistance = std::numeric_limits<float>::max();
	int rayIndex = -1;
	for (int i = 0; i < rays.size(); i++)
	{
		if (rays[i].hitDistance < lowestDistance)
		{
			lowestDistance = rays[i].hitDistance;
			rayIndex = i;
		}
	}

	if (rayIndex > -1)
	{
		ray = rays[rayIndex];
		return true;
	}

	return false;
}

bool RaycastFromScreen(Ray& ray)
{
	const int sx = editor->viewportMouseX;
	const int sy = editor->viewportMouseY;

	CameraComponent* camera = activeCamera;

	XMMATRIX proj = camera->GetProjectionMatrix();

	const float vx = (2.f * sx / Renderer::GetViewportWidth() - 1.0f) / proj.r[0].m128_f32[0];
	const float vy = (-2.f * sy / Renderer::GetViewportHeight() + 1.0f) / proj.r[1].m128_f32[1];

	ray.origin = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	ray.direction = XMVectorSet(vx, vy, 1.f, 0.f);

	constexpr float range = std::numeric_limits<float>::max();

	return Raycast(ray, ray.origin, ray.direction, range, true);
}

void Ray::AddActorsToIgnore(std::vector<Actor*>& actors)
{
	actorsToIgnore.reserve(actorsToIgnore.size() + actors.size());

	for (auto actor : actors)
	{
		actorsToIgnore.push_back(actor);
	}
}
