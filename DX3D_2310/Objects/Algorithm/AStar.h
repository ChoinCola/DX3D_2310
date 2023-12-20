#pragma once

class AStar
{
public:
    AStar(UINT width = 20, UINT height = 20);
    ~AStar();

    // Render: �׸����� ���� ���¸� ǥ���մϴ� (����� �Ǵ� �ð�ȭ ����).
    void Render();

    // SetNode: ���� ������ A* �׸��带 �ʱ�ȭ�մϴ�.
    void SetNode(Terrain* terrain);

    // FindCloseNode: �־��� ��ġ�� ������� ���� ����� ��带 ã���ϴ�.
    int FindCloseNode(Vector3 pos);

    // GetPath: ���� ��忡�� ���� �������� ���� ��θ� ã���ϴ�.
    void GetPath(IN int start, IN int end, OUT vector<Vector3>& path);

    // IsCollisionObstacle: �� ���� ���̿� ��ֹ��� �浹�� �ִ��� Ȯ���մϴ�.
    bool IsCollisionObstacle(Vector3 start, Vector3 end);

    // AddObstacle: �׸��忡 ��ֹ��� �߰��մϴ�.
    void AddObstacle(Collider* collider) { obstacles.push_back(collider); }

private:
    // Reset: A* �˰����� ���� ���¸� �ʱ�ȭ�մϴ�.
    void Reset();

    // GetDiagonalManhattanDistance: �� ��� ������ �밢 ����ư �Ÿ��� ����մϴ�.
    float GetDiagonalManhattanDistance(int start, int end);

    // Extend: ���� �߽� ��忡�� ���� ���� �˻��� Ȯ���մϴ�.
    void Extend(int center, int end);

    // GetMinNode: openNodes ��Ͽ��� �� ����� ���� ���� ��带 ã���ϴ�.
    int GetMinNode();

    // SetEdge: �׸��� ������ ��� ���� ������ �����մϴ�.
    void SetEdge();

private:
    UINT width, height;          // �׸����� ũ��.
    Vector3 interval;             // ��� ���� ����.

    vector<Node*> nodes;          // �׸��� �� ��� ����� ���.
    vector<int> openNodes;        // Ž���� ���� ���� ��� ���.

    vector<Collider*> obstacles;  // �׸��� ���� ��ֹ� ���.
};