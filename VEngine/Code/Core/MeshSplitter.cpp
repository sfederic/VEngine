#include "vpch.h"
#include "MeshSplitter.h"
#include <DirectXMath.h>
#include <DirectXCollision.h>
#include "Components/MeshComponent.h"

using namespace DirectX;

//Ref:https://itecnote.com/tecnote/c-algorithm-or-software-for-slicing-a-mesh/
//Ref:https://gdcvault.com/play/1026882/How-to-Dissect-an-Exploding
//Ref:http://simonschreibt.de/gat/metal-gear-rising-slicing/

struct Poly
{
	std::vector<Vertex> vertices;

	void AddVert(XMVECTOR pos)
	{
		Vertex v;
		XMStoreFloat3(&v.pos, pos);
		vertices.emplace_back(v);
	}
};

void TriangulatePolygons(std::vector<Poly>& polys, std::vector<Vertex>& meshVerts)
{
	for (auto& poly : polys)
	{
		assert(poly.vertices.size() > 2);
		assert(poly.vertices.size() < 5);

		//Triangulate polygons
		if (poly.vertices.size() == 4) //if an actual polygon (trapeZOIDS)
		{
			//Find closest new point
			XMVECTOR p0 = XMLoadFloat3(&poly.vertices[0].pos);
			XMVECTOR p1 = XMLoadFloat3(&poly.vertices[1].pos);

			XMVECTOR n0 = XMLoadFloat3(&poly.vertices[2].pos);
			XMVECTOR n1 = XMLoadFloat3(&poly.vertices[3].pos);

			float p0n0Dist = XMVector3Length(n0 - p0).m128_f32[0];
			float p0n1Dist = XMVector3Length(n1 - p0).m128_f32[0];

			if (p0n0Dist > p0n1Dist)
			{
				//@todo: going to have to check triangle winding using vertex's normal
				//or cheat and turn rast state to back and front.
				meshVerts.emplace_back(poly.vertices[0]);
				meshVerts.emplace_back(poly.vertices[3]);
				meshVerts.emplace_back(poly.vertices[2]);

				meshVerts.emplace_back(poly.vertices[2]);
				meshVerts.emplace_back(poly.vertices[1]);
				meshVerts.emplace_back(poly.vertices[0]);
			}
			else if (p0n0Dist < p0n1Dist)
			{
				meshVerts.emplace_back(poly.vertices[0]);
				meshVerts.emplace_back(poly.vertices[2]);
				meshVerts.emplace_back(poly.vertices[3]);

				meshVerts.emplace_back(poly.vertices[3]);
				meshVerts.emplace_back(poly.vertices[1]);
				meshVerts.emplace_back(poly.vertices[0]);
			}
		}
		else if (poly.vertices.size() == 3) //If a triangle
		{
			meshVerts.emplace_back(poly.vertices[0]);
			meshVerts.emplace_back(poly.vertices[1]);
			meshVerts.emplace_back(poly.vertices[2]);
		}
	}
}

bool CheckIntersectLine(XMVECTOR plane, XMVECTOR p0, XMVECTOR p1)
{
	XMVECTOR ba = XMVector3Normalize(p1 - p0);
	float nDotA = DirectX::XMVector3Dot(plane, p0).m128_f32[0];
	float nDotBA = DirectX::XMVector3Dot(plane, ba).m128_f32[0];

	if (nDotBA == 0.f)
	{
		return false;
	}

	return true;
}

void MeshSplitter::SplitMeshViaPlane(MeshComponent& mesh,
	std::vector<Vertex>& mesh0Verts,
	std::vector<Vertex>& mesh1Verts)
{
	//Testing plane values
	XMVECTOR planeCenter = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);
	XMVECTOR planeNormal = DirectX::XMVectorSet(1.f, 0.f, 0.f, 1.f);
	XMVECTOR plane = DirectX::XMPlaneFromPointNormal(planeCenter, planeNormal);

	//Need this because certain previous vertices will be invalid after triangle intersects.
	std::vector<Vertex> newVerts;
	
	std::vector<Poly> leftPolys;
	std::vector<Poly> rightPolys;

	for (int i = 0; i < mesh.meshDataProxy.vertices->size() / 3; i++)
	{
		const int index0 = i * 3;
		const int index1 = i * 3 + 1;
		const int index2 = i * 3 + 2;

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
			XMVECTOR line1 = DirectX::XMPlaneIntersectLine(plane, p1, p2);
			XMVECTOR line2 = DirectX::XMPlaneIntersectLine(plane, p2, p0);

			std::vector<XMVECTOR> newPoints;

			if (!DirectX::XMVectorIsNaN(line0).m128_f32[0] && CheckIntersectLine(plane, p0, p1))
			{
				newPoints.push_back(line0);
			}
			if (!DirectX::XMVectorIsNaN(line1).m128_f32[0] && CheckIntersectLine(plane, p1, p2))
			{
				newPoints.push_back(line1);
			}
			if (!DirectX::XMVectorIsNaN(line2).m128_f32[0] && CheckIntersectLine(plane, p2, p0))
			{
				newPoints.push_back(line2);
			}

			//Make sure only two edges of the triangle are split
			assert(newPoints.size() == 2);

			Poly leftPoly;
			Poly rightPoly;

			float p0Dot = XMVector3Dot(planeNormal, p0 - planeCenter).m128_f32[0];
			float p1Dot = XMVector3Dot(planeNormal, p1 - planeCenter).m128_f32[0];
			float p2Dot = XMVector3Dot(planeNormal, p2 - planeCenter).m128_f32[0];

			std::vector<std::pair<float, XMVECTOR>> dotAndPosition;

			dotAndPosition.emplace_back(std::make_pair(p0Dot, p0));
			dotAndPosition.emplace_back(std::make_pair(p1Dot, p1));
			dotAndPosition.emplace_back(std::make_pair(p2Dot, p2));

			for (auto& pair : dotAndPosition)
			{
				float dot = pair.first;
				XMVECTOR pos = pair.second;

				if (dot < 0.f) //@Todo: need to fix this and discard tris on the edge or something
				{
					leftPoly.AddVert(pos);
				}
				else if (dot > 0.f)
				{
					rightPoly.AddVert(pos);
				}
				else
				{
					throw;
				}
			}

			leftPoly.AddVert(newPoints[0]);
			leftPoly.AddVert(newPoints[1]);

			rightPoly.AddVert(newPoints[0]);
			rightPoly.AddVert(newPoints[1]);

			rightPolys.emplace_back(rightPoly);
			leftPolys.emplace_back(leftPoly);
		}
		else
		{
			//Throw original mesh vertices in if triangle doesn't intersect

			float p0Dot = XMVector3Dot(planeNormal, p0 - planeCenter).m128_f32[0];
			float p1Dot = XMVector3Dot(planeNormal, p1 - planeCenter).m128_f32[0];
			float p2Dot = XMVector3Dot(planeNormal, p2 - planeCenter).m128_f32[0];

			float dotAccum = p0Dot + p1Dot + p2Dot;
			assert(dotAccum != 0.f);
			if (dotAccum < 0.f)
			{
				mesh0Verts.emplace_back(v0);
				mesh0Verts.emplace_back(v1);
				mesh0Verts.emplace_back(v2);
			}
			else
			{
				mesh1Verts.emplace_back(v0);
				mesh1Verts.emplace_back(v1);
				mesh1Verts.emplace_back(v2);
			}
		}
	}

	std::vector<Vertex> leftMesh;
	TriangulatePolygons(leftPolys, leftMesh);
	mesh0Verts.insert(mesh0Verts.begin(), leftMesh.begin(), leftMesh.end());

	std::vector<Vertex> rightMesh;
	TriangulatePolygons(rightPolys, rightMesh);
	mesh1Verts.insert(mesh1Verts.begin(), rightMesh.begin(), rightMesh.end());
}