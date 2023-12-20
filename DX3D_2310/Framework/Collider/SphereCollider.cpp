#include "Framework.h"

SphereCollider::SphereCollider(float radius, UINT stackCount, UINT sliceCount)
	: radius(radius), stackCount(stackCount), sliceCount(sliceCount)
{
	type = SPHERE;

	MakeMesh();
	mesh->CreateMesh();
}

SphereCollider::~SphereCollider()
{
}

void SphereCollider::GUIRender()
{
	Transform::GUIRender();

	string key = tag + "_Radius";
	if (ImGui::DragFloat(key.c_str(), &radius, 0.1f))
	{
		UpdateMesh();
	}

	key = tag + "_Offset";
	if (ImGui::DragFloat3(key.c_str(), (float*)&offset, 0.1f))
	{
		UpdateMesh();
	}

	if (ImGui::Button("Save"))
		SphereSave();
	ImGui::SameLine();
	if (ImGui::Button("Load"))
		SphereLoad();
}

bool SphereCollider::IsRayCollision(IN const Ray& ray, OUT Contact* contact)
{
	Vector3 P = ray.pos;
	Vector3 D = ray.dir;

	Vector3 C = Center();
	Vector3 A = P - C;

	float b = Vector3::Dot(A, D);
	float c = Vector3::Dot(A, A) - pow(Radius(), 2);
	
	if (b * b  > c)
	{
		if (contact != nullptr)
		{
			float t = -b - sqrt(pow(b, 2) - c);

			contact->distance = t;
			contact->hitPoint = P + D * t;
		}
		return true;
	}

	return false;
}

bool SphereCollider::IsBoxCollision(BoxCollider* collider)
{
	return collider->IsSphereCollision(this);
}

bool SphereCollider::IsSphereCollision(SphereCollider* collider)
{
	float distance = (Center() - collider->Center()).Length();
	return distance < (Radius() + collider->Radius());
}

bool SphereCollider::IsCapsuleCollision(CapsuleCollider* collider)
{
	return collider->IsSphereCollision(this);
}

void SphereCollider::SphereSave()
{
	BinaryWriter* writer = new BinaryWriter("TextData/Colliders/" + tag + "_Sphere.col");
	writer->Float(radius);
	writer->Vector(offset);

	delete writer;
}

void SphereCollider::SphereLoad()
{
	BinaryReader* reader = new BinaryReader("TextData/Colliders/" + tag + "_Sphere.col");
	if (reader->IsFailed())
	{
		delete reader;
		return;
	}

	radius = reader->Float();
	offset = reader->Vector();
	UpdateMesh();

	delete reader;
}

void SphereCollider::MakeMesh()
{
	float thetaStep = XM_2PI / sliceCount;
	float phiStep = XM_PI / stackCount;

	vector<Vertex>& vertices = mesh->GetVertices();
	vertices.reserve((sliceCount + 1) * (stackCount + 1));

	for (UINT i = 0; i <= stackCount; i++)
	{
		float phi = i * phiStep;

		for (UINT j = 0; j <= sliceCount; j++)
		{
			float theta = j * thetaStep;

			Vertex vertex;
			vertex.pos.x = sin(phi) * cos(theta) * radius;
			vertex.pos.y = cos(phi) * radius;
			vertex.pos.z = sin(phi) * sin(theta) * radius;

			vertex.pos = Vector3(vertex.pos) * radius;

			vertices.push_back(vertex);
		}
	}

	// Indices
	vector<UINT>& indices = mesh->GetIndices();

	indices.reserve(sliceCount * stackCount * 4);

	for (UINT i = 0; i < stackCount; i++) {
		for (UINT j = 0; j < sliceCount; j++) {
			indices.push_back((sliceCount + 1) * i + j);	// 0
			indices.push_back((sliceCount + 1) * (i + 1) + j);	// 1

			indices.push_back((sliceCount + 1) * i + j);	// 0
			indices.push_back((sliceCount + 1) * i + j + 1);	// 2
		}
	}
}

void SphereCollider::UpdateMesh()
{
	float thetaStep = XM_2PI / sliceCount;
	float phiStep = XM_PI / stackCount;

	vector<Vertex>& vertices = mesh->GetVertices();

	for (UINT i = 0; i <= stackCount; i++)
	{
		float phi = i * phiStep;

		for (UINT j = 0; j <= sliceCount; j++)
		{
			float theta = j * thetaStep;

			Vertex vertex;
			vertex.pos.x = sin(phi) * cos(theta) * radius;
			vertex.pos.y = cos(phi) * radius;
			vertex.pos.z = sin(phi) * sin(theta) * radius;

			vertex.pos += offset;

			int index = (sliceCount + 1) * i + j;
			vertices[index] = vertex;
		}
	}

	mesh->UpdateVertices();
}
