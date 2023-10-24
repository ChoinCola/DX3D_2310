#include "Framework.h"

Sphere::Sphere(float radius, UINT sliceCount, UINT stackCount)
	: radius(radius), sliceCount(sliceCount), stackCount(stackCount)
{
	tag = "Sphere";

	mesh = new Mesh<VertexType>();
	MakeMesh();
	MakeTangent();
	mesh->CreateMesh();
}

Sphere::~Sphere()
{
	delete mesh;
}

void Sphere::Render()
{
	SetRender();
	mesh->Draw();
}

void Sphere::MakeMesh()
{
	float thetaStep = XM_2PI / sliceCount;
	float phiStep = XM_PI / stackCount;

	vector<VertexType>& vertices = mesh->GetVertices();
	vertices.reserve((sliceCount + 1) * (stackCount + 1));

	for (UINT i = 0; i <= stackCount; i++)
	{
		float phi = i * phiStep;

		for (UINT j = 0; j <= sliceCount; j++)
		{
			float theta = j * thetaStep;

			VertexType vertex;
			vertex.normal.x = sin(phi) * cos(theta);
			vertex.normal.y = cos(phi);
			vertex.normal.z = sin(phi) * sin(theta);

			vertex.pos = Vector3(vertex.normal) * radius;

			vertex.uv.x = (float)j / sliceCount;
			vertex.uv.y = (float)i / stackCount;

			vertices.push_back(vertex);
		}
	}

	// Indices
	vector<UINT>& indices = mesh->GetIndices();

	indices.reserve(sliceCount * stackCount * 6);

	for (UINT i = 0; i < stackCount; i++) {
		for (UINT j = 0; j < sliceCount; j++) {
			indices.push_back((sliceCount + 1) *	i + j);	// 0
			indices.push_back((sliceCount + 1) * i + j + 1);	// 2
			indices.push_back((sliceCount + 1) * (i + 1) + j);	// 1

			indices.push_back((sliceCount + 1) * (i + 1) + j);	// 1
			indices.push_back((sliceCount + 1) *	i + j + 1);	// 2
			indices.push_back((sliceCount + 1) * (i + 1) + j + 1);	// 3
		}
	}
}

void Sphere::MakeTangent()
{
	vector<VertexType>& vertices = mesh->GetVertices();
	vector<UINT>& indices = mesh->GetIndices();

	FOR(indices.size() / 3)
	{
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];

		Vector3 p0 = vertices[index0].pos;
		Vector3 p1 = vertices[index1].pos;
		Vector3 p2 = vertices[index2].pos;

		Float2 uv0 = vertices[index0].uv;
		Float2 uv1 = vertices[index1].uv;
		Float2 uv2 = vertices[index2].uv;

		Vector3 e0 = p1 - p0;
		Vector3 e1 = p2 - p0;

		float u0 = uv1.x - uv0.x;
		float v0 = uv1.y - uv0.y;
		float u1 = uv2.x - uv0.x;
		float v1 = uv2.y - uv0.y;

		float d = 1.0f / (u0 * v1 - v0 * u1);
		Vector3 tangent = d * (e0 * v1 - e1 * v0);

		vertices[index0].tangent += tangent;
		vertices[index1].tangent += tangent;
		vertices[index2].tangent += tangent;
	}
}
