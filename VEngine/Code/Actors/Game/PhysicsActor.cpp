#include "PhysicsActor.h"
#include "Components/MeshComponent.h"
#include "Physics/PhysicsSystem.h"
#include "Physics/Raycast.h"
#include "VMath.h"

PhysicsActor::PhysicsActor()
{
	//gravityVector = -VMath::XMVectorUp();
	//moveVector = gravityVector;

	//sphereMesh = MeshComponent::system.Add(this, MeshComponent("sphere.fbx", "test.png"));
	//rootComponent = sphereMesh;

	baseMesh = MeshComponent::system.Add(this, MeshComponent("cube.fbx", "test.png"));
	baseMesh->skipPhysicsCreation = true;
	rootComponent = baseMesh;
}

void PhysicsActor::Start()
{
	physicsSystem.CreateConvexPhysicsMeshFromCollisionMesh(baseMesh, this, "hull_cube.fbx");
}

bool PhysicsActor::GoOverAllTris()
{
	XMVECTOR sphereCenter = GetPositionVector();
	const float radius = 1.0f;

	for (auto mesh : MeshComponent::system.components)
	{
		if (mesh == baseMesh)
		{
			continue;
		}

		auto model = mesh->GetWorldMatrix();

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

			XMVECTOR triangleNormal = XMVector3Normalize(XMVector3Cross(v0 - v1, v2 - v0));
			float dist = XMVector3Dot(sphereCenter - v0, triangleNormal).m128_f32[0];

			if (dist < -radius || dist > radius)
			{
				continue;
			}

			moveVector = XMVector3Reflect(gravityVector, triangleNormal);

			return true;
		}
	}

	return false;
}

void PhysicsActor::Tick(float deltaTime)
{
	//GoOverAllTris();

	//auto pos = GetPositionVector();
	//pos += moveVector * (deltaTime);
	//SetPosition(pos);
}

Properties PhysicsActor::GetProps()
{
	return __super::GetProps();
}
