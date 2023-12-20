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

// 캡슐(CapsuleCollider)과 광선(Ray) 간의 충돌 여부를 검사하는 함수
bool CapsuleCollider::IsRayCollision(IN const Ray& ray, OUT Contact* contact)
{
	// 캡슐 충돌선의 방향 벡터와 시작점, 끝점 계산
	Vector3 direction = GetUp();
	Vector3 pa = Center() - direction * Height() * 0.5f;
	Vector3 pb = Center() + direction * Height() * 0.5f;

	float r = Radius(); // 캡슐의 반지름

	// 광선의 원점과 방향 벡터
	Vector3 ro = ray.pos;
	Vector3 rd = ray.dir;

	// 캡슐 충돌선 벡터 및 기타 계산
	// 캡슐 충돌선 벡터 계산: 캡슐의 끝점(pb)에서 시작점(pa)으로 향하는 벡터
	Vector3 ba = pb - pa;

	// 광선 원점에서 캡슐 시작점으로 향하는 벡터
	Vector3 oa = ro - pa;

	// 캡슐 충돌선 벡터의 길이의 제곱
	float baba = Vector3::Dot(ba, ba);

	// 캡슐 충돌선 벡터와 광선 방향 벡터의 내적
	float bard = Vector3::Dot(ba, rd);

	// 캡슐 충돌선 벡터와 광선 원점에서 출발한 벡터의 내적
	float baoa = Vector3::Dot(ba, oa);

	// 광선 방향 벡터와 광선 원점에서 출발한 벡터의 내적
	float rdoa = Vector3::Dot(rd, oa);

	// 광선 원점에서 출발한 벡터의 길이의 제곱
	float oaoa = Vector3::Dot(oa, oa);

	// 이차방정식의 계수 계산
	float a = baba - bard * bard;
	float b = baba * rdoa - baoa * bard;
	float c = baba * oaoa - baoa * baoa - r * r * baba;

	// 판별식 계산
	float h = b * b - a * c;

	if (h >= 0.0f)
	{
		// 충돌이 발생한 경우
		float t = (-b - sqrt(h)) / a;

		float distance = baoa + t * bard;

		if (distance > 0.0f && distance < baba)
		{
			// 충돌이 발생하고 충돌 지점 계산
			if (contact)
			{
				contact->distance = distance;
				contact->hitPoint = ray.pos + ray.dir * t;
			}

			return true;
		}

		// 충돌이 출발점의 뒤쪽에서 발생한 경우
		Vector3 oc = (distance <= 0.0f) ? oa : ro - pb;
		b = Vector3::Dot(rd, oc);
		c = Vector3::Dot(oc, oc) - r * r;
		h = b * b - c;

		if (h > 0.0f)
		{
			// 충돌이 발생하고 충돌 지점 계산
			if (contact)
			{
				contact->distance = distance;
				contact->hitPoint = ray.pos + ray.dir * t;
			}

			return true;
		}
	}

	// 충돌이 발생하지 않은 경우
	return false;
}

// 주어진 상자(BoxCollider)와 캡슐 간의 충돌 여부를 검사하는 함수
bool CapsuleCollider::IsBoxCollision(BoxCollider* collider)
{
	// 주어진 상자(BoxCollider)의 정보 얻기
	BoxCollider::ObbDesc box;
	collider->GetObb(box);

	// 현재 캡슐의 방향 벡터와 시작점, 끝점 계산
	Vector3 direction = GetUp();
	Vector3 pa = Center() - direction * Height() * 0.5f; // 시작점
	Vector3 pb = Center() + direction * Height() * 0.5f; // 끝점

	// 캡슐 충돌선상에서 상자에 가장 가까운 점과 해당 점이 속한 축 찾기
	Vector3 closestPointToSphere = box.pos;
	Vector3 pointOnLine = MATH->ClosestPointOnLine(pa, pb, box.pos);
	Vector3 boxToSphere = pointOnLine - box.pos;

	// 각 축에 대해 가장 가까운 지점 찾기
	FOR(3)
	{
		float length = Vector3::Dot(box.axis[i], boxToSphere);
		float mult = (length < 0.0f) ? -1.0f : 1.0f;
		length = min(abs(length), box.halfSize[i]);
		closestPointToSphere += box.axis[i] * length * mult;
	}

	// 캡슐 충돌선상의 가장 가까운 점과 상자의 가장 가까운 점 사이의 거리 계산
	float distance = (pointOnLine - closestPointToSphere).Length();

	// 계산된 거리가 캡슐의 반지름 이하이면 충돌이 발생한 것으로 판단
	return distance <= Radius();
}

bool CapsuleCollider::IsSphereCollision(SphereCollider* collider)
{
	// 캡슐 충돌 검사를 위한 방향 벡터 구하기
	Vector3 direction = GetUp();

	// 캡슐의 시작점과 끝점 계산
	Vector3 pa = Center() - direction * Height() * 0.5f; // 시작점
	Vector3 pb = Center() + direction * Height() * 0.5f; // 끝점

	// 구의 중심점 위치 구하기
	Vector3 P = collider->Center();

	// 캡슐의 충돌선 상에서 구에 가장 가까운 점 계산
	Vector3 pointOnLine = MATH->ClosestPointOnLine(pa, pb, P);

	// 구의 중심점과 캡슐 충돌선 위의 가장 가까운 점 사이의 거리 계산
	float distance = (P - pointOnLine).Length();

	// 계산된 거리가 두 콜라이더의 반지름 합 이하이면 충돌이 발생한 것으로 판단
	return distance <= (Radius() + collider->Radius());
}

// 두 캡슐 간의 충돌 여부를 검사하는 함수
bool CapsuleCollider::IsCapsuleCollision(CapsuleCollider* collider)
{
	// 첫 번째 캡슐의 방향 벡터와 시작점, 끝점 계산
	Vector3 aDirection = GetUp();
	Vector3 aA = Center() - aDirection * Height() * 0.5f; // 시작점
	Vector3 aB = Center() + aDirection * Height() * 0.5f; // 끝점

	// 두 번째 캡슐의 방향 벡터와 시작점, 끝점 계산
	Vector3 bDirection = collider->GetUp();
	Vector3 bA = collider->Center() - bDirection * collider->Height() * 0.5f; // 시작점
	Vector3 bB = collider->Center() + bDirection * collider->Height() * 0.5f; // 끝점

	// 각 캡슐의 네 개의 꼭짓점에 대한 벡터 계산
	Vector3 v0 = bA - aA;
	Vector3 v1 = bB - aA;
	Vector3 v2 = bA - aB;
	Vector3 v3 = bB - aB;

	// 각 꼭짓점 사이의 거리의 제곱 계산
	float d0 = Vector3::Dot(v0, v0);
	float d1 = Vector3::Dot(v1, v1);
	float d2 = Vector3::Dot(v2, v2);
	float d3 = Vector3::Dot(v3, v3);

	// 각 꼭짓점에서 가장 가까운 캡슐과의 교차점 계산
	Vector3 bestA;
	if (d2 < d0 || d2 < d1 || d3 < d0 || d3 > d1)
		bestA = aB;
	else
		bestA = aA;

	Vector3 bestB = MATH->ClosestPointOnLine(bA, bB, bestA);
	bestA = MATH->ClosestPointOnLine(aA, aB, bestB);
	bestB = MATH->ClosestPointOnLine(bA, bB, bestA);

	// 두 캡슐 사이의 최소 거리 계산
	float distance = (bestA - bestB).Length();

	// 계산된 거리가 두 캡슐의 반지름 합 이하이면 충돌이 발생한 것으로 판단
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
