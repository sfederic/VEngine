#include "Raycast.h"
#include <limits>
#include "Camera.h"
#include "Render/Renderer.h"
#include "Editor/Editor.h"
#include "Math.h"
#include "Actors/IActorSystem.h"
#include "Actors/Actor.h"
#include "Components/MeshComponent.h"
#include "World.h"

#undef max

//export void DrawRayDebug(XMVECTOR rayOrigin, XMVECTOR rayDir, float distance)
//{
//	Line debugLine = {};
//	XMStoreFloat3(&debugLine.p1.pos, rayOrigin);
//	XMVECTOR pos = XMLoadFloat3(&debugLine.p1.pos);
//	pos += GetActiveCamera()->right;
//	XMStoreFloat3(&debugLine.p1.pos, pos);
//
//	if (distance <= 0.f)
//	{
//		distance = 0.f;
//	}
//
//	XMVECTOR dist = rayDir * distance;
//	XMVECTOR rayEnd = rayOrigin + dist;
//	XMStoreFloat3(&debugLine.p2.pos, rayEnd);
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

	for (Actor* actor : world.GetAllActorsInWorld())
	{
		//Iterate over actor's spatial components
		for (SpatialComponent* spatialComponent : actor->GetComponentsOfType<SpatialComponent>())
		{
			BoundingOrientedBox boundingBox = spatialComponent->boundingBox;

			boundingBox.Center = spatialComponent->transform.position;
			boundingBox.Orientation = spatialComponent->transform.rotation;
			boundingBox.Extents.x *= spatialComponent->transform.scale.x;
			boundingBox.Extents.y *= spatialComponent->transform.scale.y;
			boundingBox.Extents.z *= spatialComponent->transform.scale.z;

			if (boundingBox.Intersects(ray.origin, ray.direction, ray.hitDistance))
			{
				distances.push_back(ray.hitDistance);
				ray.hitActors.push_back(actor);
				bRayHit = true;
			}
		}
	}

	//Figure out the distance
	if (bRayHit)
	{
		float nearestDistance = std::numeric_limits<float>::max();
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

				ray.hitActor = ray.hitActors[i];

				//Skip raycast if actor is in ignore list
				for (Actor* actorToIgnore : ray.actorsToIgnore)
				{
					if (actorToIgnore == ray.hitActors[i])
					{
						ray.hitActor = nullptr;
						return false;
					}
				}
			}
		}

		ray.hitDistance = nearestDistance;

		return true;
	}

	return false;
}

bool RaycastTriangleIntersect(Ray& ray)
{
	std::vector<Ray> rays;

	for (Actor* actor : ray.hitActors)
	{
		XMMATRIX model = actor->GetTransformMatrix();

		for (Component* component : actor->components)
		{
			MeshComponent* mesh = dynamic_cast<MeshComponent*>(component);
			if (mesh)
			{
				for (int i = 0; i < mesh->data->vertices->size() / 3; i++)
				{
					uint32_t index0 = mesh->data->indices->at(i * 3);
					uint32_t index1 = mesh->data->indices->at(i * 3 + 1);
					uint32_t index2 = mesh->data->indices->at(i * 3 + 2);

					XMVECTOR v0 = XMLoadFloat3(&mesh->data->vertices->at(index0).pos);
					v0 = XMVector3TransformCoord(v0, model);

					XMVECTOR v1 = XMLoadFloat3(&mesh->data->vertices->at(index1).pos);
					v1 = XMVector3TransformCoord(v1, model);

					XMVECTOR v2 = XMLoadFloat3(&mesh->data->vertices->at(index2).pos);
					v2 = XMVector3TransformCoord(v2, model);

					Ray tempRay = {};
					tempRay = ray;
					tempRay.hitActors.clear();

					if (DirectX::TriangleTests::Intersects(ray.origin, ray.direction, v0, v1, v2, tempRay.hitDistance))
					{
						XMVECTOR normal = XMVectorZero();
						normal += XMLoadFloat3(&mesh->data->vertices->at(index0).normal);
						normal += XMLoadFloat3(&mesh->data->vertices->at(index1).normal);
						normal += XMLoadFloat3(&mesh->data->vertices->at(index2).normal);

						normal = XMVector3Normalize(normal);

						XMStoreFloat3(&tempRay.normal, normal);

						tempRay.hitActor = actor;

						rays.push_back(tempRay);
					}
				}
			}
		}
	}

	float lowestDistance = D3D11_FLOAT32_MAX;
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

	const float vx = (2.f * sx / renderer.GetViewportWidth() - 1.0f) / camera->proj.r[0].m128_f32[0];
	const float vy = (-2.f * sy / renderer.GetViewportHeight() + 1.0f) / camera->proj.r[1].m128_f32[1];

	ray.origin = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	ray.direction = XMVectorSet(vx, vy, 1.f, 0.f);

	const float range = std::numeric_limits<float>::max();

	return Raycast(ray, ray.origin, ray.direction, range, true);
}
