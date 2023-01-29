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

	//Need this because certain previous vertices will be invalid after triangle intersects.
	std::vector<Vertex> newVerts;
	
	std::vector<Triangle> newTris;

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

		if (intersectType == PlaneIntersectionType::INTERSECTING)
		{
			XMVECTOR line0 = DirectX::XMPlaneIntersectLine(plane, p0, p1);
			XMVECTOR line1 = DirectX::XMPlaneIntersectLine(plane, p0, p2);
			XMVECTOR line2 = DirectX::XMPlaneIntersectLine(plane, p1, p2);

			std::vector<XMVECTOR> newPoints;

			if (!DirectX::XMVectorIsNaN(line0).m128_f32[0]) newPoints.push_back(line0);
			if (!DirectX::XMVectorIsNaN(line1).m128_f32[0]) newPoints.push_back(line1);
			if (!DirectX::XMVectorIsNaN(line2).m128_f32[0]) newPoints.push_back(line2);

			//line0 with tri0 and tri2 are where vertices that's aren't 'hit', the NaN result.
			//Seperate that side out when creating new triangles. Every triangle split will generate 3 new tris.

			Triangle newTri0{};
			Triangle newTri1{};
			Triangle newTri2{};

			XMStoreFloat3(&newTri0.v0.pos, p0);
			XMStoreFloat3(&newTri0.v1.pos, newPoints[0]);
			XMStoreFloat3(&newTri0.v2.pos, newPoints[1]);

			XMStoreFloat3(&newTri1.v0.pos, p0);
			XMStoreFloat3(&newTri1.v1.pos, newPoints[0]);
			XMStoreFloat3(&newTri1.v2.pos, p2);

			XMStoreFloat3(&newTri2.v0.pos, p2);
			XMStoreFloat3(&newTri2.v1.pos, newPoints[0]);
			XMStoreFloat3(&newTri2.v2.pos, newPoints[1]);

			//Push back new triangles
			newVerts.emplace_back(newTri0.v0);
			newVerts.emplace_back(newTri0.v1);
			newVerts.emplace_back(newTri0.v2);

			newVerts.emplace_back(newTri1.v0);
			newVerts.emplace_back(newTri1.v1);
			newVerts.emplace_back(newTri1.v2);

			newVerts.emplace_back(newTri2.v0);
			newVerts.emplace_back(newTri2.v1);
			newVerts.emplace_back(newTri2.v2);

			//new triangles
			newTris.emplace_back(newTri0);
			newTris.emplace_back(newTri1);
			newTris.emplace_back(newTri2);
		}
		else
		{
			//Push back old triangles
			newVerts.emplace_back(v0);
			newVerts.emplace_back(v1);
			newVerts.emplace_back(v2);

			Triangle oldTri{};
			oldTri.v0 = v0;
			oldTri.v1 = v1;
			oldTri.v2 = v2;
			newTris.emplace_back(oldTri);
		}
	}

	std::vector<Vertex> newMesh0;
	std::vector<Vertex> newMesh1;

	//split tris by which side of the plane they're on
	for (auto& tri : newTris)
	{
		XMVECTOR v0 = XMLoadFloat3(&tri.v0.pos);
		XMVECTOR v1 = XMLoadFloat3(&tri.v1.pos);
		XMVECTOR v2 = XMLoadFloat3(&tri.v2.pos);

		float dot1 = XMVector3Dot(planeNormal, v0 - planeCenter).m128_f32[0];
		float dot2 = XMVector3Dot(planeNormal, v1 - planeCenter).m128_f32[0];
		float dot3 = XMVector3Dot(planeNormal, v2 - planeCenter).m128_f32[0];

		float dotAccum = dot1 + dot2 + dot3;
		//The 0 here worries me. Is there a way to give the vertices a small offset or something?
		if (dotAccum >= 0.f) 
		{
			newMesh0.emplace_back(tri.v0);
			newMesh0.emplace_back(tri.v1);
			newMesh0.emplace_back(tri.v2);
		}
		else
		{
			newMesh1.emplace_back(tri.v0);
			newMesh1.emplace_back(tri.v1);
			newMesh1.emplace_back(tri.v2);
		}
	}

	return;
}
