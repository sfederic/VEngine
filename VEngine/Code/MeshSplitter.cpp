#include "vpch.h"
#include "MeshSplitter.h"
#include <DirectXMath.h>
#include <DirectXCollision.h>
#include "Components/MeshComponent.h"
#include "Render/RenderTypes.h"

using namespace DirectX;

void MeshSplitter::SplitMeshViaPlane(MeshComponent& mesh)
{
	//Testing plane values
	XMVECTOR planeCenter = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);
	XMVECTOR planeNormal = DirectX::XMVectorSet(1.f, 0.f, 0.f, 1.f);
	XMVECTOR plane = DirectX::XMPlaneFromPointNormal(planeCenter, planeNormal);

	std::vector<Vertex> newVerts;

	for (int i = 0; i < mesh.meshDataProxy.vertices->size() / 3; i++)
	{
		int index0 = i * 3;
		int index1 = i * 3 + 1;
		int index2 = i * 3 + 2;

		Vertex v0 = mesh.meshDataProxy.vertices->at(index0);
		Vertex v1 = mesh.meshDataProxy.vertices->at(index1);
		Vertex v2 = mesh.meshDataProxy.vertices->at(index2);

		XMVECTOR p0 = XMLoadFloat3(&v0.pos);
		XMVECTOR p1 = XMLoadFloat3(&v1.pos);
		XMVECTOR p2 = XMLoadFloat3(&v2.pos);

		PlaneIntersectionType intersectType = DirectX::TriangleTests::Intersects(p0, p1, p2, plane);

		std::vector<XMVECTOR> newPoints;

		if (intersectType == PlaneIntersectionType::INTERSECTING)
		{
			XMVECTOR line0 = DirectX::XMPlaneIntersectLine(plane, p0, p1);
			XMVECTOR line1 = DirectX::XMPlaneIntersectLine(plane, p0, p2);
			XMVECTOR line2 = DirectX::XMPlaneIntersectLine(plane, p1, p2);

			if (!DirectX::XMVectorIsNaN(line0).m128_f32[0]) newPoints.push_back(line0);
			if (!DirectX::XMVectorIsNaN(line1).m128_f32[0]) newPoints.push_back(line1);
			if (!DirectX::XMVectorIsNaN(line2).m128_f32[0]) newPoints.push_back(line2);

			//sect1 with tri0 and tri2 are where vertices that's aren't 'hit', the NaN result.
			//Seperate that side out when creating new triangles.

			Vertex newTri0[3]{};
			Vertex newTri1[3]{};
			Vertex newTri2[3]{};

			XMStoreFloat3(&newTri0[0].pos, p0);
			XMStoreFloat3(&newTri0[1].pos, newPoints[0]);
			XMStoreFloat3(&newTri0[2].pos, newPoints[1]);

			XMStoreFloat3(&newTri1[0].pos, p0);
			XMStoreFloat3(&newTri1[1].pos, newPoints[0]);
			XMStoreFloat3(&newTri1[2].pos, p2);

			XMStoreFloat3(&newTri2[0].pos, p2);
			XMStoreFloat3(&newTri2[1].pos, newPoints[0]);
			XMStoreFloat3(&newTri2[2].pos, newPoints[1]);

			newVerts.emplace_back(newTri0);
			newVerts.emplace_back(newTri1);
			newVerts.emplace_back(newTri2);
		}
	}

	std::vector<Vertex> verticesLeftOfPlane;
	std::vector<Vertex> verticesRightOfPlane;

	//sort tris by which side of the plane they're on
	/*for (auto& vertex : newVerts)
	{
		float dot1 = XMVector3Dot(planeNormal, tri.v0 - planeCenter).m128_f32[0];
		float dot2 = XMVector3Dot(planeNormal, tri.v1 - planeCenter).m128_f32[0];
		float dot3 = XMVector3Dot(planeNormal, tri.v2 - planeCenter).m128_f32[0];

		float dotAccum = dot1 + dot2 + dot3;
		if (dotAccum >= 0.f)
		{
			trisLeftOfPlane.emplace_back(tri);
		}
		else
		{
			trisRightOfPlane.emplace_back(tri);
		}
	}

	for (auto& tri : tris)
	{
		float dot1 = XMVector3Dot(planeNormal, tri.v0 - planeCenter).m128_f32[0];
		float dot2 = XMVector3Dot(planeNormal, tri.v1 - planeCenter).m128_f32[0];
		float dot3 = XMVector3Dot(planeNormal, tri.v2 - planeCenter).m128_f32[0];

		float dotAccum = dot1 + dot2 + dot3;
		if (dotAccum >= 0.f)
		{
			trisLeftOfPlane.emplace_back(tri);
		}
		else
		{
			trisRightOfPlane.emplace_back(tri);
		}
	}*/
}
