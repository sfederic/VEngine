#include "Raycast.h"
#include "RenderSystem.h"
#include "Camera.h"
#include "UISystem.h"
#include "Actor.h"
#include "DebugMenu.h"
#include "CoreSystem.h"
#include "Debug.h"
#include "World.h"
#include "WorldEditor.h"
#include "PropertiesWidget.h"
#include "PropertiesDock.h"
#include "../EditorMainWindow.h"

void DrawRayDebug(XMVECTOR rayOrigin, XMVECTOR rayDir, float distance, class ID3D11Buffer* debugBuffer)
{
	Vertex v1 = {}, v2 = {};
	XMStoreFloat3(&v1.pos, rayOrigin);
	if (distance <= 0.f)
	{
		distance = 10000.f;
	}
	XMVECTOR dist = rayDir * distance;
	XMVECTOR rayEnd = rayOrigin + dist;
	XMStoreFloat3(&v2.pos, rayEnd);

	//gRenderSystem.debugLines.push_back(v1);
	//gRenderSystem.debugLines.push_back(v2);

	//gRenderSystem.context->UpdateSubresource(debugBuffer, 0, nullptr, gRenderSystem.debugLines.data(), 0, 0);
}

bool Raycast(Ray& ray, XMVECTOR origin, XMVECTOR direction, ActorSystem* actorSystem)
{
	ray.origin = origin;
	ray.direction = direction;

	Camera* camera = GetActiveCamera();

	XMMATRIX invView = XMMatrixInverse(&XMMatrixDeterminant(camera->view), camera->view);
	XMMATRIX invModel = XMMatrixInverse(&XMMatrixDeterminant(XMMatrixIdentity()), XMMatrixIdentity());
	XMMATRIX toLocal = XMMatrixMultiply(invView, invModel);

	ray.origin = XMVector3TransformCoord(ray.origin, toLocal);
	ray.direction = XMVector3TransformNormal(ray.direction, toLocal);
	ray.direction = XMVector3Normalize(ray.direction);

	std::vector<float> distances;
	std::vector<int> actorIndices;

	bool bRayHit = false;

	for (int i = 0; i < actorSystem->actors.size(); i++)
	{
		XMFLOAT3 offset = XMFLOAT3(
			actorSystem->actors[i]->GetPositionFloat3().x + actorSystem->boundingBox.Center.x,
			actorSystem->actors[i]->GetPositionFloat3().y + actorSystem->boundingBox.Center.y,
			actorSystem->actors[i]->GetPositionFloat3().z + actorSystem->boundingBox.Center.z
		);


		BoundingBox tempBoundingBox = actorSystem->boundingBox;
		//There's a temp bounding box because the previous offset was always being added to the Actorsystem bounding box
		tempBoundingBox.Center = offset;

		//For editor exit
		if (actorSystem->actors[i]->bPicked)
		{
			actorSystem->actors[i]->bPicked = false;
			return false;
		}

		//Skip if actor is not render as well (good for translation widgets)
		if (!actorSystem->actors[i]->bRender)
		{
			return false;
		}

		if (tempBoundingBox.Intersects(ray.origin, ray.direction, ray.distance))
		{
			distances.push_back(ray.distance);
			actorIndices.push_back(i);

			ray.actorIndex = i;

			bRayHit = true;
		}
	}

	if (bRayHit)
	{
		float nearestDistance = D3D11_FLOAT32_MAX;
		for (int i = 0; i < distances.size(); i++)
		{
			if (distances[i] < nearestDistance)
			{
				nearestDistance = distances[i];
				ray.actorIndex = actorIndices[i];
			}
		}

		return true;
	}

	return false;
}

bool RaycastTriangleIntersect(Ray& ray)
{
	ActorSystem* actorSystem = GetWorld()->GetActorSystem(ray.actorSystemIndex);
	assert(actorSystem);
	
	Actor* actor = actorSystem->GetActor(ray.actorIndex);
	assert(actor);

	XMMATRIX model = actor->GetTransformationMatrix();
	XMMATRIX mvp = model * gRenderSystem.matrices.view * gRenderSystem.matrices.proj;

	std::vector<Ray> rays;
	
	for (int i = 0; i < actorSystem->modelData.verts.size() / 3; i++)
	{
		XMVECTOR v0 = XMLoadFloat3(&actorSystem->modelData.verts[i * 3 + 0].pos);
		XMVector3TransformCoord(v0, mvp);

		XMVECTOR v1 = XMLoadFloat3(&actorSystem->modelData.verts[i * 3 + 1].pos);
		XMVector3TransformCoord(v1, mvp);

		XMVECTOR v2 = XMLoadFloat3(&actorSystem->modelData.verts[i * 3 + 2].pos);
		XMVector3TransformCoord(v2, mvp);


		Ray tempRay = {};
		tempRay = ray;

		if (DirectX::TriangleTests::Intersects(ray.origin, ray.direction, v0, v1, v2, tempRay.distance))
		{
			tempRay.modelDataIndex = i;

			XMVECTOR normal = XMVectorZero();
			normal += XMLoadFloat3(&actorSystem->modelData.verts[i * 3 + 0].normal);
			normal += XMLoadFloat3(&actorSystem->modelData.verts[i * 3 + 1].normal);
			normal += XMLoadFloat3(&actorSystem->modelData.verts[i * 3 + 2].normal);

			normal = XMVector3Normalize(normal);

			XMStoreFloat3(&tempRay.normal, normal);

			rays.push_back(tempRay);

			DebugPrint("%s Triangle index %d hit.\n", typeid(actor).name(), i);
		}
	}


	float lowestDistance = D3D11_FLOAT32_MAX;
	int rayIndex = -1;
	for(int i = 0; i < rays.size(); i++)
	{
		if (rays[i].distance < lowestDistance)
		{
			lowestDistance = rays[i].distance;
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

bool RaycastAll(Ray& ray, XMVECTOR origin, XMVECTOR direction, World* world)
{
	for (int i = 0; i < world->actorSystems.size(); i++)
	{
		if (Raycast(ray, origin, direction, world->actorSystems[i]))
		{
			ray.actorSystemIndex = i;
			return true;
		}
	}

	return false;
}

bool RaycastFromScreen(Ray& ray, int sx, int sy, Camera* camera, ActorSystem* actorSystem)
{
	float vx = (2.f * sx / gCoreSystem.windowWidth - 1.0f) / camera->proj.r[0].m128_f32[0];
	float vy = (-2.f * sy / gCoreSystem.windowHeight + 1.0f) / camera->proj.r[1].m128_f32[1];

	ray.origin = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	ray.direction = XMVectorSet(vx, vy, 1.f, 0.f);

	return Raycast(ray, ray.origin, ray.direction, actorSystem);
}

bool RaycastAllFromScreen(Ray& ray, int sx, int sy, Camera* camera, World* world)
{
	for (int i = 0; i < world->actorSystems.size(); i++)
	{
		if (RaycastFromScreen(ray, sx, sy, camera, world->actorSystems[i]))
		{
			ray.actorSystemIndex = i;

			Actor* actor = world->GetActor(ray.actorSystemIndex, ray.actorIndex);
			if (actor)
			{
				gWorldEditor.pickedActor = actor;
			}

			return true;
		}
	}

	return false;
}
