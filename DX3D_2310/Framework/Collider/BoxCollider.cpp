#include "Framework.h"

BoxCollider::BoxCollider(Vector3 size)
    : size(size)
{
    type = BOX;

    MakeMesh();
    mesh->CreateMesh();
}

bool BoxCollider::IsRayCollision(IN const Ray& ray, OUT Contact* contact)
{
    ObbDesc box;
    GetObb(box); // 박스의 정보를 얻어옵니다.

    // Obb의 경계를 나타낸다. 레이의 충돌를 확인하는데 사용된다.
    Vector3 min = box.halfSize * -1.0f; // 박스의 최소 꼭짓점을 계산합니다.
    Vector3 max = box.halfSize; // 박스의 최대 꼭짓점을 계산합니다.

    // OBB와 레이의 상대위치를 나타냄
    Vector3 delta = box.pos - ray.pos; // 레이의 출발점과 박스 중심 사이의 벡터를 계산합니다.
    Vector3 D = ray.dir.GetNormalized(); // 레이의 방향을 단위 벡터로 정규화합니다. (레이 방향이 0벡터이면 에러가 발생할 수 있음.)

    // 후속 계산시 업데이트하며, 충돌시점 추적하는데 사용함.
    float tMin = 0.0f; // 충돌 시점의 최소값을 초기화합니다.
    float tMax = FLT_MAX; // 충돌 시점의 최대값을 FLT_MAX로 초기화합니다. (실수의 최대값)

    FOR(3) // 0부터 2까지 반복합니다.
    {
        Vector3 axis = box.axis[i]; // 박스의 로컬 축을 가져옵니다.
        float e = Vector3::Dot(axis, delta); // 박스의 축과 delta 벡터의 내적을 계산합니다.
        float f = Vector3::Dot(axis, D); // 박스의 축과 레이의 방향 벡터의 내적을 계산합니다.

        // 위에서 밑으로 수직으로 바라보았을때 해당된다.
        if (MATH->NearlyEqual(e, f)) // e와 f가 거의 같으면 (레이와 축이 평행할 때) 
        {
            if (min[i] > e || max[i] < e) return false; // 박스와 레이가 평행하면서 박스의 최소 또는 최대값을 벗어나면 충돌이 없습니다.
        }

        // 시작지점에서 교차된다.
        else // e와 f가 다른 경우 (레이와 축이 교차할 때)
        {
            float t1 = (e + min[i]) / f; // t1과 t2는 레이가 박스와 교차하는 지점의 보간값입니다.
            float t2 = (e + max[i]) / f;

            if (t1 > t2) swap(t1, t2); // t1이 항상 작은 값이 되도록 정렬합니다.

            if (t2 < tMax) tMax = t2; // 최대 충돌 시점을 업데이트합니다.
            if (t1 > tMin) tMin = t1; // 최소 충돌 시점을 업데이트합니다.

            if (tMin > tMax) return false; // 최소 충돌 시점이 최대 충돌 시점보다 크면 레이와 박스는 겹치지 않습니다.
        }
    }

    // 충돌 최외각지점을 contact의 hitPoint에 저장한다.
    if (contact != nullptr)
    {
        contact->distance = tMin; // 충돌 시점의 거리를 저장합니다.
        contact->hitPoint = (Vector3)ray.pos + ray.dir * tMin; // 충돌 지점을 계산하여 저장합니다.
    }

    return true; // 레이와 박스가 교차하면 true를 반환합니다.
}

bool BoxCollider::IsBoxCollision(BoxCollider* collider)
{
    ObbDesc box1, box2; // 두 개의 OBB(축 정렬된 바운딩 박스)를 선언합니다.

    GetObb(box1); // 첫 번째 박스의 정보를 얻어옵니다.
    collider->GetObb(box2); // 두 번째 박스의 정보를 얻어옵니다.

    Vector3 D = box2.pos - box1.pos; // 두 박스 중심 사이의 거리 벡터 D를 계산합니다.

    FOR(3) // i를 0부터 2까지 반복합니다. (FOR은 반복문을 나타냅니다.)
    {
        // 각 x,y,z축 기준의 axis로 바라보았을 때 두개의 사각형 사이의 거리를 계산하고 그걸 기준으로 충돌검사를 진행한다.
        if (IsSeperateAxis(D, box1.axis[i], box1, box2)) return false; // 충돌 축이 있는지 검사합니다.
        if (IsSeperateAxis(D, box2.axis[i], box1, box2)) return false; // 두 박스 간의 충돌 축이 있는지 다시 검사합니다.
    }

    // 축이 같은것이 한개라도 존재한다면, (벡터가 수직할때), 충돌축이 없을경우, 충돌하지 않는다
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (box1.axis[i] == box2.axis[j]) return true; // 두 박스의 축 중 어느 것이 같은지 확인하여 충돌 여부를 결정합니다.
        }
    }

    // 겹치는 축이 하나도 없다. 그러면 외적을 다시 계산한다.
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            Vector3 cross = Vector3::Cross(box1.axis[i], box2.axis[j]); // 두 박스 축 간의 외적을 계산합니다.
            if (IsSeperateAxis(D, cross, box1, box2)) return false; // 외적 축으로 다시 충돌 검사를 수행합니다.
        }
    }

    return true; // 결과가 나타나지 않을경우 충돌로 확인한다.
}

bool BoxCollider::IsSphereCollision(SphereCollider* collider)
{
    ObbDesc box;
    GetObb(box); // OBB 정보를 얻어와서 box에 저장합니다.

    Vector3 closestPointToSphere = box.pos; // OBB의 중심을 기준으로 가장 가까운 지점을 초기화합니다.

    FOR(3) // OBB의 3개의 로컬 축에 대한 루프를 시작합니다.
    {
        Vector3 direction = collider->GetGlobalPosition() - box.pos; // OBB 중심에서 충돌체의 중심까지의 방향을 계산합니다.

        float length = Vector3::Dot(box.axis[i], direction); // 충돌체 중심과 OBB 축 사이의 내적을 계산하여 OBB 축 방향으로의 거리(length)를 얻습니다.

        float mult = (length < 0.0f) ? -1.0f : 1.0f; // 거리의 방향을 결정합니다. OBB 축 방향과 반대 방향인 경우 -1, 같은 방향인 경우 1을 사용합니다.

        length = min(abs(length), box.halfSize[i]); // OBB 축 방향으로의 거리(length)를 OBB의 halfSize와 비교하여 더 작은 값을 선택합니다.

        closestPointToSphere += box.axis[i] * length * mult; // 가장 가까운 지점을 업데이트합니다. 이 지점은 OBB의 표면 위의 지점입니다.
    }

    float distance = (collider->GetGlobalPosition() - closestPointToSphere).Length(); // 충돌체의 중심과 OBB 표면의 가장 가까운 지점 사이의 거리를 계산합니다.
    collider->Getdistance() = distance;
    collider->GetHitpoint() = closestPointToSphere;
    return distance <= collider->Radius(); // 계산된 거리가 충돌체의 반지름보다 작거나 같으면 충돌이 발생한 것으로 간주합니다.
}

bool BoxCollider::IsCapsuleCollision(CapsuleCollider* collider)
{
    return false;
}

void BoxCollider::GetObb(ObbDesc& obbDesc)
{
    obbDesc.pos = GetGlobalPosition(); // 상속을 많이 받아서 월드로 계산한다.

    obbDesc.axis[0] = GetRight();
    obbDesc.axis[1] = GetUp();
    obbDesc.axis[2] = GetForward();

    obbDesc.halfSize = size * 0.5f * GetGlobalScale();
}

void BoxCollider::MakeMesh()
{
    Vector3 halfSize = size * 0.5;

    vector<Vertex>& vertices = mesh->GetVertices();

    //Front
    vertices.emplace_back(-halfSize.x, -halfSize.y, -halfSize.z);
    vertices.emplace_back(-halfSize.x, +halfSize.y, -halfSize.z);
    vertices.emplace_back(+halfSize.x, -halfSize.y, -halfSize.z);
    vertices.emplace_back(+halfSize.x, +halfSize.y, -halfSize.z);

    //Back
    vertices.emplace_back(-halfSize.x, -halfSize.y, +halfSize.z);
    vertices.emplace_back(-halfSize.x, +halfSize.y, +halfSize.z);
    vertices.emplace_back(+halfSize.x, -halfSize.y, +halfSize.z);
    vertices.emplace_back(+halfSize.x, +halfSize.y, +halfSize.z);


    vector<UINT>& indices = mesh->GetIndices();

    /*
        1        3
        ----------
        1        1
        1        1
        1        1
        ----------
        0        2
    */

    indices =
    {
        0, 1, 1, 3, 3, 2, 2, 0,
        4, 5, 5, 7, 7, 6, 6, 4,
        0, 4, 1, 5, 3, 7, 2, 6
    };
}

bool BoxCollider::IsSeperateAxis(const Vector3 D, const Vector3 axis, const ObbDesc& box1, const ObbDesc& box2)
{
    // 각 축에 대하여 투영된 거리를 계산하여. 모든 축에대해 OBB를 계산하는 함수이다.
    // 1. 축에 대한 거리를 계산합니다.
    float distance = abs(Vector3::Dot(D, axis));

    // 2. 각 박스의 축을 따라서 해당 축에 투영된 크기를 계산합니다.
    float a = 0.0f; // 첫 번째 박스의 투영된 크기 를 담을 float
    float b = 0.0f; // 두 번째 박스의 투영된 크기 를 담을 float

    // 3. 각 박스의 3개 축에 대해 반복합니다.
    FOR(3)
    {
        // 4. 현재 축에 해당하는 박스의 반향 크기를 해당 축과의 내적으로 계산합니다.
        Vector3 temp = box1.axis[i] * box1.halfSize[i];
        a += abs(Vector3::Dot(temp, axis));

        // 5. 두 번째 박스에 대해서도 동일한 작업을 수행합니다.
        temp = box2.axis[i] * box2.halfSize[i];
        b += abs(Vector3::Dot(temp, axis));
    }

    // 6. 거리가 두 박스의 투영된 크기 합보다 큰 경우, 축에 대한 간격이 있으므로 두 박스는 겹치지 않습니다.
    return distance > (a + b);
}
