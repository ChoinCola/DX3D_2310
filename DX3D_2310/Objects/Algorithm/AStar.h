#pragma once

class AStar
{
public:
    AStar(UINT width = 20, UINT height = 20);
    ~AStar();

    // Render: 그리드의 현재 상태를 표시합니다 (디버깅 또는 시각화 목적).
    void Render();

    // SetNode: 지형 정보로 A* 그리드를 초기화합니다.
    void SetNode(Terrain* terrain);

    // FindCloseNode: 주어진 위치를 기반으로 가장 가까운 노드를 찾습니다.
    int FindCloseNode(Vector3 pos);

    // GetPath: 시작 노드에서 종단 노드까지의 최적 경로를 찾습니다.
    void GetPath(IN int start, IN int end, OUT vector<Vector3>& path);

    // IsCollisionObstacle: 두 지점 사이에 장애물과 충돌이 있는지 확인합니다.
    bool IsCollisionObstacle(Vector3 start, Vector3 end);

    // AddObstacle: 그리드에 장애물을 추가합니다.
    void AddObstacle(Collider* collider) { obstacles.push_back(collider); }

private:
    // Reset: A* 알고리즘의 내부 상태를 초기화합니다.
    void Reset();

    // GetDiagonalManhattanDistance: 두 노드 사이의 대각 맨해튼 거리를 계산합니다.
    float GetDiagonalManhattanDistance(int start, int end);

    // Extend: 현재 중심 노드에서 종단 노드로 검색을 확장합니다.
    void Extend(int center, int end);

    // GetMinNode: openNodes 목록에서 총 비용이 가장 작은 노드를 찾습니다.
    int GetMinNode();

    // SetEdge: 그리드 내에서 노드 간의 엣지를 설정합니다.
    void SetEdge();

private:
    UINT width, height;          // 그리드의 크기.
    Vector3 interval;             // 노드 간의 간격.

    vector<Node*> nodes;          // 그리드 내 모든 노드의 목록.
    vector<int> openNodes;        // 탐색을 위한 열린 노드 목록.

    vector<Collider*> obstacles;  // 그리드 내의 장애물 목록.
};