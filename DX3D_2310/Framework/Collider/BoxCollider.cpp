#include "Framework.h"

BoxCollider::BoxCollider(Vector3 size)
    : size(size)
{
    type = BOX;

    MakeMesh();
    mesh->CreateMesh();
}

void BoxCollider::GUIRender()
{
    Transform::GUIRender();

    string key = tag + "_Size";
    if (ImGui::DragFloat3(key.c_str(), (float*)&size, 0.1f))
    {
        UpdateMesh();
    }

    key = tag + "_Offset";
    if (ImGui::DragFloat3(key.c_str(), (float*)&offset, 0.1f))
    {
        UpdateMesh();
    }

    if (ImGui::Button("Save"))
        BoxSave();
    ImGui::SameLine();
    if (ImGui::Button("Load"))
        BoxLoad();
}

bool BoxCollider::IsRayCollision(IN const Ray& ray, OUT Contact* contact)
{
    ObbDesc box;
    GetObb(box); // �ڽ��� ������ ���ɴϴ�.

    // Obb�� ��踦 ��Ÿ����. ������ �浹�� Ȯ���ϴµ� ���ȴ�.
    Vector3 min = box.halfSize * -1.0f; // �ڽ��� �ּ� �������� ����մϴ�.
    Vector3 max = box.halfSize; // �ڽ��� �ִ� �������� ����մϴ�.

    // OBB�� ������ �����ġ�� ��Ÿ��
    Vector3 delta = box.pos - ray.pos; // ������ ������� �ڽ� �߽� ������ ���͸� ����մϴ�.
    Vector3 D = ray.dir.GetNormalized(); // ������ ������ ���� ���ͷ� ����ȭ�մϴ�. (���� ������ 0�����̸� ������ �߻��� �� ����.)

    // �ļ� ���� ������Ʈ�ϸ�, �浹���� �����ϴµ� �����.
    float tMin = 0.0f; // �浹 ������ �ּҰ��� �ʱ�ȭ�մϴ�.
    float tMax = FLT_MAX; // �浹 ������ �ִ밪�� FLT_MAX�� �ʱ�ȭ�մϴ�. (�Ǽ��� �ִ밪)

    FOR(3) // 0���� 2���� �ݺ��մϴ�.
    {
        Vector3 axis = box.axis[i]; // �ڽ��� ���� ���� �����ɴϴ�.
        float e = Vector3::Dot(axis, delta); // �ڽ��� ��� delta ������ ������ ����մϴ�.
        float f = Vector3::Dot(axis, D); // �ڽ��� ��� ������ ���� ������ ������ ����մϴ�.

        // ������ ������ �������� �ٶ󺸾����� �ش�ȴ�.
        if (MATH->NearlyEqual(e, f)) // e�� f�� ���� ������ (���̿� ���� ������ ��) 
        {
            if (min[i] > e || max[i] < e) return false; // �ڽ��� ���̰� �����ϸ鼭 �ڽ��� �ּ� �Ǵ� �ִ밪�� ����� �浹�� �����ϴ�.
        }

        // ������������ �����ȴ�.
        else // e�� f�� �ٸ� ��� (���̿� ���� ������ ��)
        {
            float t1 = (e + min[i]) / f; // t1�� t2�� ���̰� �ڽ��� �����ϴ� ������ �������Դϴ�.
            float t2 = (e + max[i]) / f;

            if (t1 > t2) swap(t1, t2); // t1�� �׻� ���� ���� �ǵ��� �����մϴ�.

            if (t2 < tMax) tMax = t2; // �ִ� �浹 ������ ������Ʈ�մϴ�.
            if (t1 > tMin) tMin = t1; // �ּ� �浹 ������ ������Ʈ�մϴ�.

            if (tMin > tMax) return false; // �ּ� �浹 ������ �ִ� �浹 �������� ũ�� ���̿� �ڽ��� ��ġ�� �ʽ��ϴ�.
        }
    }

    // �浹 �ֿܰ������� contact�� hitPoint�� �����Ѵ�.
    if (contact != nullptr)
    {
        contact->distance = tMin; // �浹 ������ �Ÿ��� �����մϴ�.
        contact->hitPoint = (Vector3)ray.pos + ray.dir * tMin; // �浹 ������ ����Ͽ� �����մϴ�.
    }

    return true; // ���̿� �ڽ��� �����ϸ� true�� ��ȯ�մϴ�.
}

bool BoxCollider::IsBoxCollision(BoxCollider* collider)
{
    ObbDesc box1, box2; // �� ���� OBB(�� ���ĵ� �ٿ�� �ڽ�)�� �����մϴ�.

    GetObb(box1); // ù ��° �ڽ��� ������ ���ɴϴ�.
    collider->GetObb(box2); // �� ��° �ڽ��� ������ ���ɴϴ�.

    Vector3 D = box2.pos - box1.pos; // �� �ڽ� �߽� ������ �Ÿ� ���� D�� ����մϴ�.

    FOR(3) // i�� 0���� 2���� �ݺ��մϴ�. (FOR�� �ݺ����� ��Ÿ���ϴ�.)
    {
        // �� x,y,z�� ������ axis�� �ٶ󺸾��� �� �ΰ��� �簢�� ������ �Ÿ��� ����ϰ� �װ� �������� �浹�˻縦 �����Ѵ�.
        if (IsSeperateAxis(D, box1.axis[i], box1, box2)) return false; // �浹 ���� �ִ��� �˻��մϴ�.
        if (IsSeperateAxis(D, box2.axis[i], box1, box2)) return false; // �� �ڽ� ���� �浹 ���� �ִ��� �ٽ� �˻��մϴ�.
    }

    // ���� �������� �Ѱ��� �����Ѵٸ�, (���Ͱ� �����Ҷ�), �浹���� �������, �浹���� �ʴ´�
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (box1.axis[i] == box2.axis[j]) return true; // �� �ڽ��� �� �� ��� ���� ������ Ȯ���Ͽ� �浹 ���θ� �����մϴ�.
        }
    }

    // ��ġ�� ���� �ϳ��� ����. �׷��� ������ �ٽ� ����Ѵ�.
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            Vector3 cross = Vector3::Cross(box1.axis[i], box2.axis[j]); // �� �ڽ� �� ���� ������ ����մϴ�.
            if (IsSeperateAxis(D, cross, box1, box2)) return false; // ���� ������ �ٽ� �浹 �˻縦 �����մϴ�.
        }
    }

    return true; // ����� ��Ÿ���� ������� �浹�� Ȯ���Ѵ�.
}

bool BoxCollider::IsSphereCollision(SphereCollider* collider)
{
    ObbDesc box;
    GetObb(box); // OBB ������ ���ͼ� box�� �����մϴ�.

    Vector3 closestPointToSphere = box.pos; // OBB�� �߽��� �������� ���� ����� ������ �ʱ�ȭ�մϴ�.

    FOR(3) // OBB�� 3���� ���� �࿡ ���� ������ �����մϴ�.
    {
        Vector3 direction = collider->Center() - box.pos; // OBB �߽ɿ��� �浹ü�� �߽ɱ����� ������ ����մϴ�.

        float length = Vector3::Dot(box.axis[i], direction); // �浹ü �߽ɰ� OBB �� ������ ������ ����Ͽ� OBB �� ���������� �Ÿ�(length)�� ����ϴ�.

        float mult = (length < 0.0f) ? -1.0f : 1.0f; // �Ÿ��� ������ �����մϴ�. OBB �� ����� �ݴ� ������ ��� -1, ���� ������ ��� 1�� ����մϴ�.

        length = min(abs(length), box.halfSize[i]); // OBB �� ���������� �Ÿ�(length)�� OBB�� halfSize�� ���Ͽ� �� ���� ���� �����մϴ�.

        closestPointToSphere += box.axis[i] * length * mult; // ���� ����� ������ ������Ʈ�մϴ�. �� ������ OBB�� ǥ�� ���� �����Դϴ�.
    }

    float distance = (collider->Center() - closestPointToSphere).Length(); // �浹ü�� �߽ɰ� OBB ǥ���� ���� ����� ���� ������ �Ÿ��� ����մϴ�.
    collider->Getdistance() = distance;
    collider->GetHitpoint() = closestPointToSphere;
    return distance <= collider->Radius(); // ���� �Ÿ��� �浹ü�� ���������� �۰ų� ������ �浹�� �߻��� ������ �����մϴ�.
}

bool BoxCollider::IsCapsuleCollision(CapsuleCollider* collider)
{
    return collider->IsBoxCollision(this);
}

void BoxCollider::GetObb(ObbDesc& obbDesc)
{
    obbDesc.pos = Center(); // ����� ���� �޾Ƽ� ����� ����Ѵ�.

    obbDesc.axis[0] = GetRight();
    obbDesc.axis[1] = GetUp();
    obbDesc.axis[2] = GetForward();

    obbDesc.halfSize = size * 0.5f * GetGlobalScale();
}

void BoxCollider::BoxSave()
{
    BinaryWriter* writer = new BinaryWriter("TextData/Colliders/" + tag + "_Box.col");
    writer->Vector(size);
    writer->Vector(offset);

    delete writer;
}

void BoxCollider::BoxLoad()
{
    BinaryReader* reader = new BinaryReader("TextData/Colliders/" + tag + "_Box.col");
    if (reader->IsFailed())
    {
        delete reader;
        return;
    }

    size = reader->Vector();
    offset = reader->Vector();
    UpdateMesh();

    delete reader;
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

void BoxCollider::UpdateMesh()
{
    Vector3 halfSize = size * 0.5;

    vector<Vertex>& vertices = mesh->GetVertices();

    //Front
    vertices[0] = Vertex(-halfSize.x + offset.x, -halfSize.y + offset.y, -halfSize.z + offset.z);
    vertices[1] = Vertex(-halfSize.x + offset.x, +halfSize.y + offset.y, -halfSize.z + offset.z);
    vertices[2] = Vertex(+halfSize.x + offset.x, -halfSize.y + offset.y, -halfSize.z + offset.z);
    vertices[3] = Vertex(+halfSize.x + offset.x, +halfSize.y + offset.y, -halfSize.z + offset.z);
    vertices[4] = Vertex(-halfSize.x + offset.x, -halfSize.y + offset.y, +halfSize.z + offset.z);
    vertices[5] = Vertex(-halfSize.x + offset.x, +halfSize.y + offset.y, +halfSize.z + offset.z);
    vertices[6] = Vertex(+halfSize.x + offset.x, -halfSize.y + offset.y, +halfSize.z + offset.z);
    vertices[7] = Vertex(+halfSize.x + offset.x, +halfSize.y + offset.y, +halfSize.z + offset.z);
    
    mesh->UpdateVertices();
}

bool BoxCollider::IsSeperateAxis(const Vector3 D, const Vector3 axis, const ObbDesc& box1, const ObbDesc& box2)
{
    // �� �࿡ ���Ͽ� ������ �Ÿ��� ����Ͽ�. ��� �࿡���� OBB�� ����ϴ� �Լ��̴�.
    // 1. �࿡ ���� �Ÿ��� ����մϴ�.
    float distance = abs(Vector3::Dot(D, axis));

    // 2. �� �ڽ��� ���� ���� �ش� �࿡ ������ ũ�⸦ ����մϴ�.
    float a = 0.0f; // ù ��° �ڽ��� ������ ũ�� �� ���� float
    float b = 0.0f; // �� ��° �ڽ��� ������ ũ�� �� ���� float

    // 3. �� �ڽ��� 3�� �࿡ ���� �ݺ��մϴ�.
    FOR(3)
    {
        // 4. ���� �࿡ �ش��ϴ� �ڽ��� ���� ũ�⸦ �ش� ����� �������� ����մϴ�.
        Vector3 temp = box1.axis[i] * box1.halfSize[i];
        a += abs(Vector3::Dot(temp, axis));

        // 5. �� ��° �ڽ��� ���ؼ��� ������ �۾��� �����մϴ�.
        temp = box2.axis[i] * box2.halfSize[i];
        b += abs(Vector3::Dot(temp, axis));
    }

    // 6. �Ÿ��� �� �ڽ��� ������ ũ�� �պ��� ū ���, �࿡ ���� ������ �����Ƿ� �� �ڽ��� ��ġ�� �ʽ��ϴ�.
    return distance > (a + b);
}
