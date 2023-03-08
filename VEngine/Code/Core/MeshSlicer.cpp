#include "vpch.h"
#include "MeshSlicer.h"
#include <DirectXMath.h>
#include <DirectXCollision.h>
#include "Components/MeshComponent.h"
#include "Render/Vertex.h"

using namespace DirectX;

//Ref:https://itecnote.com/tecnote/c-algorithm-or-software-for-slicing-a-mesh/
//Ref:https://gdcvault.com/play/1026882/How-to-Dissect-an-Exploding
//Ref:http://simonschreibt.de/gat/metal-gear-rising-slicing/

//@Todo: mesh slice will fail if it gets cut down the middle straight along triangle edges
//as it will return multiple new verticesalong that edge.

Vertex InterpolateVerts(const Vertex& v0, const Vertex& v1, XMVECTOR pos)
{
	XMVECTOR v0Pos = XMLoadFloat3(&v0.pos);
	XMVECTOR v1Pos = XMLoadFloat3(&v1.pos);

	float posTov0PosLength = XMVector3Length(pos - v0Pos).m128_f32[0];
	float v0Tov1Length = XMVector3Length(v0Pos - v1Pos).m128_f32[0];
	float uvInterpValue = posTov0PosLength / v0Tov1Length;

	//Ignore skeletal data
	Vertex o;
	XMStoreFloat3(&o.pos, pos);
	o.normal = v0.normal; //Normal and tangents will be the same as it's all the same triangle
	o.tangent = v0.tangent;
	XMVECTOR uvInterp = XMVectorLerp(XMLoadFloat2(&v0.uv), XMLoadFloat2(&v1.uv), uvInterpValue);
	XMStoreFloat2(&o.uv, uvInterp);

	return o;
}

struct Poly
{
	std::vector<Vertex> vertices;

	void AddVert(Vertex& v)
	{
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

bool CheckIntersectLine(XMVECTOR planeNormal, XMVECTOR planeCenter, XMVECTOR p0, XMVECTOR p1)
{
	XMVECTOR u = p1 - p0;
	XMVECTOR w = p0 - planeCenter;

	float D = DirectX::XMVector3Dot(planeNormal, u).m128_f32[0];
	float N = -DirectX::XMVector3Dot(planeNormal, w).m128_f32[0];

	constexpr float e = std::numeric_limits<float>::epsilon();
	float ab = std::abs(D);
	if (ab < e)
	{
		// segment is parallel to plane
		if (N == 0) // segment lies in plane
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	float sI = N / D;
	if (sI < 0 || sI > 1)
	{
		return false;
	}

	return true;
}

void TriangulateNewVerts(std::vector<Vertex>& remainingNewVerts)
{
	//Use this vector to delete from (remember to not remove the initial vert at [0]
	std::vector<Vertex> verts = remainingNewVerts;

	//Start all triangle fans from this initial vertex
	Vertex initialVert = verts.front();

	std::vector<Vertex> newTriangleFan;
	newTriangleFan.push_back(initialVert);

	//Roll back triangle positions when this hits 2
	int indexCounter = 0;

	float shortestDistance = std::numeric_limits<float>::max();
	int shortestDistIndex = 0;

	XMVECTOR p0 = XMLoadFloat3(&initialVert.pos);

	for (int i = 0; i < verts.size(); i++)
	{
		for (int j = 0; j < verts.size(); j++)
		{
			XMVECTOR p1 = XMLoadFloat3(&verts[j].pos);

			float dist = XMVector3Length(p1 - p0).m128_f32[0];
			if (dist == 0)
			{
				continue;
			}
			if (dist < shortestDistance)
			{
				shortestDistance = dist;
				shortestDistIndex = j;
			}
		}

		assert(shortestDistIndex != 0);
		newTriangleFan.push_back(verts[shortestDistIndex]);

		indexCounter++;
		if (indexCounter == 2)
		{
			newTriangleFan.push_back(initialVert);
			newTriangleFan.push_back(verts[shortestDistIndex]);

			verts.erase(verts.begin() + shortestDistIndex);

			indexCounter = 0;
		}
		else
		{
			verts.erase(verts.begin() + shortestDistIndex);
		}
	}

	remainingNewVerts = newTriangleFan;
}

void RemoveDuplicateNewVerts(std::vector<Vertex>& newVerts)
{
	for (int i = 0; i < newVerts.size(); i++)
	{
		XMVECTOR p0 = XMLoadFloat3(&newVerts[i].pos);

		for (int j = 0; j < newVerts.size(); j++)
		{
			XMVECTOR p1 = XMLoadFloat3(&newVerts[j].pos);

			if (XMVector4Equal(p0, p1))
			{
				newVerts.erase(newVerts.begin() + i);
			}
		}
	}
}

void MeshSlicer::SliceMeshViaPlane(DirectX::XMVECTOR planeCenter,
	DirectX::XMVECTOR planeNormal,
	MeshComponent& mesh,
	std::vector<Vertex>& mesh0Verts,
	std::vector<Vertex>& mesh1Verts)
{
	planeNormal = XMVector3Normalize(planeNormal);
	XMVECTOR plane = DirectX::XMPlaneFromPointNormal(planeCenter, planeNormal);

	std::vector<Vertex> allNewVerts;
	
	std::vector<Poly> leftPolys;
	std::vector<Poly> rightPolys;

	const XMMATRIX meshWorldMatrix = mesh.GetWorldMatrix();

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

		//Transform positions into world space from Mesh's matrix
		p0 = XMVector3TransformCoord(p0, meshWorldMatrix);
		p1 = XMVector3TransformCoord(p1, meshWorldMatrix);
		p2 = XMVector3TransformCoord(p2, meshWorldMatrix);

		//Store transformed positions back into vertices
		XMStoreFloat3(&v0.pos, p0);
		XMStoreFloat3(&v1.pos, p1);
		XMStoreFloat3(&v2.pos, p2);

		PlaneIntersectionType intersectType = DirectX::TriangleTests::Intersects(p0, p1, p2, plane);

		if (intersectType == PlaneIntersectionType::INTERSECTING)
		{
			XMVECTOR line0 = DirectX::XMPlaneIntersectLine(plane, p0, p1);
			XMVECTOR line1 = DirectX::XMPlaneIntersectLine(plane, p1, p2);
			XMVECTOR line2 = DirectX::XMPlaneIntersectLine(plane, p2, p0);

			std::vector<Vertex> newVerts;

			//@Todo: some meshes aren't working when they're split right on the 
			//nose and multiple vertices are kissing the same edges.

			if (!DirectX::XMVectorIsNaN(line0).m128_f32[0] && CheckIntersectLine(planeNormal, planeCenter, p0, p1))
			{
				Vertex v = InterpolateVerts(v0, v1, line0);
				newVerts.push_back(v);
				allNewVerts.push_back(v);
			}
			if (!DirectX::XMVectorIsNaN(line1).m128_f32[0] && CheckIntersectLine(planeNormal, planeCenter, p1, p2))
			{
				Vertex v = InterpolateVerts(v1, v2, line1);
				newVerts.push_back(v);
				allNewVerts.push_back(v);
			}
			if (!DirectX::XMVectorIsNaN(line2).m128_f32[0] && CheckIntersectLine(planeNormal, planeCenter, p2, p0))
			{
				Vertex v = InterpolateVerts(v2, v0, line2);
				newVerts.push_back(v);
				allNewVerts.push_back(v);
			}

			//Make sure only two edges of the triangle are split
			assert(newVerts.size() == 2);

			Poly leftPoly;
			Poly rightPoly;

			float p0Dot = XMVector3Dot(planeNormal, p0 - planeCenter).m128_f32[0];
			float p1Dot = XMVector3Dot(planeNormal, p1 - planeCenter).m128_f32[0];
			float p2Dot = XMVector3Dot(planeNormal, p2 - planeCenter).m128_f32[0];

			std::vector<std::pair<float, Vertex>> dotAndVertex;

			dotAndVertex.push_back(std::make_pair(p0Dot, v0));
			dotAndVertex.push_back(std::make_pair(p1Dot, v1));
			dotAndVertex.push_back(std::make_pair(p2Dot, v2));

			for (auto& [dot, vertex] : dotAndVertex)
			{
				if (dot < 0.f)
				{
					leftPoly.AddVert(vertex);
				}
				else if (dot >= 0.f)
				{
					rightPoly.AddVert(vertex);
				}
			}

			leftPoly.AddVert(newVerts[0]);
			leftPoly.AddVert(newVerts[1]);

			rightPoly.AddVert(newVerts[0]);
			rightPoly.AddVert(newVerts[1]);

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

	//RemoveDuplicateNewVerts(allNewVerts);
	//TriangulateNewVerts(allNewVerts);

	//Because the new triangle slices need to be in world space, re-transform the vertices
	//back to their original local space.
	XMVECTOR invDet = XMMatrixDeterminant(XMMatrixIdentity());
	XMMATRIX inverseMeshMatrix = XMMatrixInverse(&invDet, mesh.GetWorldMatrix());

	std::vector<Vertex> leftMesh;
	TriangulatePolygons(leftPolys, leftMesh);
	mesh0Verts.insert(mesh0Verts.begin(), leftMesh.begin(), leftMesh.end());
	//mesh0Verts.insert(mesh0Verts.end(), allNewVerts.begin(), allNewVerts.end());
	for (auto& v : mesh0Verts)
	{
		XMVECTOR p = XMLoadFloat3(&v.pos);
		p = XMVector3TransformCoord(p, inverseMeshMatrix);
		XMStoreFloat3(&v.pos, p);
	}

	std::vector<Vertex> rightMesh;
	TriangulatePolygons(rightPolys, rightMesh);
	mesh1Verts.insert(mesh1Verts.begin(), rightMesh.begin(), rightMesh.end());
	//mesh1Verts.insert(mesh1Verts.end(), allNewVerts.begin(), allNewVerts.end());
	for (auto& v : mesh1Verts)
	{
		XMVECTOR p = XMLoadFloat3(&v.pos);
		p = XMVector3TransformCoord(p, inverseMeshMatrix);
		XMStoreFloat3(&v.pos, p);
	}
}
