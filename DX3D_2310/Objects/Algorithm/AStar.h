#pragma once

class AStar
{
public:
    AStar(UINT width = 20, UINT height = 20);
    ~AStar();

    void Render();

    void SetNode(Terrain* terrain);

    int FindCloseNode(Vector3 pos);

    void GetPath(IN int start, IN int end, OUT vector<Vector3>& path);

    bool IsCollisionObstacle(Vector3 start, Vector3 end);

    void AddObstacle(Collider* collider) { obstacles.push_back(collider); }

private:
    void Reset();

    float GetDiagonalManhattanDistance(int start, int end);

    void Extend(int center, int end);
    int GetMinNode();

    void SetEdge();

private:
    UINT width, height;
    Vector3 interval;

    vector<Node*> nodes;
    vector<int> openNodes;

    vector<Collider*> obstacles;
};