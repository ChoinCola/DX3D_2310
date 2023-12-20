#include "Framework.h"

CapsuleCollider::CapsuleCollider(float radius, float height, UINT stackCount, UINT sliceCount)
    : radius(radius),height(height), stackCount(stackCount), sliceCount(sliceCount)
{
    type = CAPSULE;

    MakeMesh();
    mesh->CreateMesh();
}

void CapsuleCollider::GUIRender()
{
	Transform::GUIRender();

	string key = tag + "_Radius";
	if (ImGui::DragFloat(key.c_str(), &radius, 0.1f))
	{
		UpdateMesh();
	}

	key = tag + "_Height";
	if (ImGui::DragFloat(key.c_str(), &height, 0.1f))
	{
		UpdateMesh();
	}

	key = tag + "_Offset";
	if (ImGui::DragFloat3(key.c_str(), (float*)&offset, 0.1f))
	{
		UpdateMesh();
	}

	if (ImGui::Button("Save"))
		CapsuleSave();
	ImGui::SameLine();
	if (ImGui::Button("Load"))
		CapsuleLoad();
}

// ĸ��(CapsuleCollider)�� ����(Ray) ���� �浹 ���θ� �˻��ϴ� �Լ�
bool CapsuleCollider::IsRayCollision(IN const Ray& ray, OUT Contact* contact)
{
	// ĸ�� �浹���� ���� ���Ϳ� ������, ���� ���
	Vector3 direction = GetUp();
	Vector3 pa = Center() - direction * Height() * 0.5f;
	Vector3 pb = Center() + direction * Height() * 0.5f;

	float r = Radius(); // ĸ���� ������

	// ������ ������ ���� ����
	Vector3 ro = ray.pos;
	Vector3 rd = ray.dir;

	// ĸ�� �浹�� ���� �� ��Ÿ ���
	// ĸ�� �浹�� ���� ���: ĸ���� ����(pb)���� ������(pa)���� ���ϴ� ����
	Vector3 ba = pb - pa;

	// ���� �������� ĸ�� ���������� ���ϴ� ����
	Vector3 oa = ro - pa;

	// ĸ�� �浹�� ������ ������ ����
	float baba = Vector3::Dot(ba, ba);

	// ĸ�� �浹�� ���Ϳ� ���� ���� ������ ����
	float bard = Vector3::Dot(ba, rd);

	// ĸ�� �浹�� ���Ϳ� ���� �������� ����� ������ ����
	float baoa = Vector3::Dot(ba, oa);

	// ���� ���� ���Ϳ� ���� �������� ����� ������ ����
	float rdoa = Vector3::Dot(rd, oa);

	// ���� �������� ����� ������ ������ ����
	float oaoa = Vector3::Dot(oa, oa);

	// ������������ ��� ���
	float a = baba - bard * bard;
	float b = baba * rdoa - baoa * bard;
	float c = baba * oaoa - baoa * baoa - r * r * baba;

	// �Ǻ��� ���
	float h = b * b - a * c;

	if (h >= 0.0f)
	{
		// �浹�� �߻��� ���
		float t = (-b - sqrt(h)) / a;

		float distance = baoa + t * bard;

		if (distance > 0.0f && distance < baba)
		{
			// �浹�� �߻��ϰ� �浹 ���� ���
			if (contact)
			{
				contact->distance = distance;
				contact->hitPoint = ray.pos + ray.dir * t;
			}

			return true;
		}

		// �浹�� ������� ���ʿ��� �߻��� ���
		Vector3 oc = (distance <= 0.0f) ? oa : ro - pb;
		b = Vector3::Dot(rd, oc);
		c = Vector3::Dot(oc, oc) - r * r;
		h = b * b - c;

		if (h > 0.0f)
		{
			// �浹�� �߻��ϰ� �浹 ���� ���
			if (contact)
			{
				contact->distance = distance;
				contact->hitPoint = ray.pos + ray.dir * t;
			}

			return true;
		}
	}

	// �浹�� �߻����� ���� ���
	return false;
}

// �־��� ����(BoxCollider)�� ĸ�� ���� �浹 ���θ� �˻��ϴ� �Լ�
bool CapsuleCollider::IsBoxCollision(BoxCollider* collider)
{
	// �־��� ����(BoxCollider)�� ���� ���
	BoxCollider::ObbDesc box;
	collider->GetObb(box);

	// ���� ĸ���� ���� ���Ϳ� ������, ���� ���
	Vector3 direction = GetUp();
	Vector3 pa = Center() - direction * Height() * 0.5f; // ������
	Vector3 pb = Center() + direction * Height() * 0.5f; // ����

	// ĸ�� �浹���󿡼� ���ڿ� ���� ����� ���� �ش� ���� ���� �� ã��
	Vector3 closestPointToSphere = box.pos;
	Vector3 pointOnLine = MATH->ClosestPointOnLine(pa, pb, box.pos);
	Vector3 boxToSphere = pointOnLine - box.pos;

	// �� �࿡ ���� ���� ����� ���� ã��
	FOR(3)
	{
		float length = Vector3::Dot(box.axis[i], boxToSphere);
		float mult = (length < 0.0f) ? -1.0f : 1.0f;
		length = min(abs(length), box.halfSize[i]);
		closestPointToSphere += box.axis[i] * length * mult;
	}

	// ĸ�� �浹������ ���� ����� ���� ������ ���� ����� �� ������ �Ÿ� ���
	float distance = (pointOnLine - closestPointToSphere).Length();

	// ���� �Ÿ��� ĸ���� ������ �����̸� �浹�� �߻��� ������ �Ǵ�
	return distance <= Radius();
}

bool CapsuleCollider::IsSphereCollision(SphereCollider* collider)
{
	// ĸ�� �浹 �˻縦 ���� ���� ���� ���ϱ�
	Vector3 direction = GetUp();

	// ĸ���� �������� ���� ���
	Vector3 pa = Center() - direction * Height() * 0.5f; // ������
	Vector3 pb = Center() + direction * Height() * 0.5f; // ����

	// ���� �߽��� ��ġ ���ϱ�
	Vector3 P = collider->Center();

	// ĸ���� �浹�� �󿡼� ���� ���� ����� �� ���
	Vector3 pointOnLine = MATH->ClosestPointOnLine(pa, pb, P);

	// ���� �߽����� ĸ�� �浹�� ���� ���� ����� �� ������ �Ÿ� ���
	float distance = (P - pointOnLine).Length();

	// ���� �Ÿ��� �� �ݶ��̴��� ������ �� �����̸� �浹�� �߻��� ������ �Ǵ�
	return distance <= (Radius() + collider->Radius());
}

// �� ĸ�� ���� �浹 ���θ� �˻��ϴ� �Լ�
bool CapsuleCollider::IsCapsuleCollision(CapsuleCollider* collider)
{
	// ù ��° ĸ���� ���� ���Ϳ� ������, ���� ���
	Vector3 aDirection = GetUp();
	Vector3 aA = Center() - aDirection * Height() * 0.5f; // ������
	Vector3 aB = Center() + aDirection * Height() * 0.5f; // ����

	// �� ��° ĸ���� ���� ���Ϳ� ������, ���� ���
	Vector3 bDirection = collider->GetUp();
	Vector3 bA = collider->Center() - bDirection * collider->Height() * 0.5f; // ������
	Vector3 bB = collider->Center() + bDirection * collider->Height() * 0.5f; // ����

	// �� ĸ���� �� ���� �������� ���� ���� ���
	Vector3 v0 = bA - aA;
	Vector3 v1 = bB - aA;
	Vector3 v2 = bA - aB;
	Vector3 v3 = bB - aB;

	// �� ������ ������ �Ÿ��� ���� ���
	float d0 = Vector3::Dot(v0, v0);
	float d1 = Vector3::Dot(v1, v1);
	float d2 = Vector3::Dot(v2, v2);
	float d3 = Vector3::Dot(v3, v3);

	// �� ���������� ���� ����� ĸ������ ������ ���
	Vector3 bestA;
	if (d2 < d0 || d2 < d1 || d3 < d0 || d3 > d1)
		bestA = aB;
	else
		bestA = aA;

	Vector3 bestB = MATH->ClosestPointOnLine(bA, bB, bestA);
	bestA = MATH->ClosestPointOnLine(aA, aB, bestB);
	bestB = MATH->ClosestPointOnLine(bA, bB, bestA);

	// �� ĸ�� ������ �ּ� �Ÿ� ���
	float distance = (bestA - bestB).Length();

	// ���� �Ÿ��� �� ĸ���� ������ �� �����̸� �浹�� �߻��� ������ �Ǵ�
	return distance <= (Radius() + collider->Radius());
}

void CapsuleCollider::CapsuleSave()
{
	BinaryWriter* writer = new BinaryWriter("TextData/Colliders/" + tag + "_Capsule.col");
	writer->Float(radius);
	writer->Float(height);
	writer->Vector(offset);

	delete writer;
}

void CapsuleCollider::CapsuleLoad()
{
	BinaryReader* reader = new BinaryReader("TextData/Colliders/" + tag + "_Capsule.col");
	if (reader->IsFailed())
	{
		delete reader;
		return;
	}

	radius = reader->Float();
	height = reader->Float();
	offset = reader->Vector();
	UpdateMesh();

	delete reader;
}

void CapsuleCollider::MakeMesh()
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

			if (vertex.pos.y > 0.0f)
				vertex.pos.y += height * 0.5f;
			else if (vertex.pos.y < 0.0f)
				vertex.pos.y -= height * 0.5f;

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

void CapsuleCollider::UpdateMesh()
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

			if (vertex.pos.y > 0)
				vertex.pos.y += height * 0.5f;
			else if (vertex.pos.y < 0)
				vertex.pos.y -= height * 0.5f;

			vertex.pos += offset;

			int index = (sliceCount + 1) * i + j;
			vertices[index] = vertex;
		}
	}

	mesh->UpdateVertices();
}
