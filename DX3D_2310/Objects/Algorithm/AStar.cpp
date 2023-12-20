#include "Framework.h"

AStar::AStar(UINT width, UINT height)
    : width(width), height(height)
{
}

AStar::~AStar()
{
    // 동적으로 할당된 노드들을 정리합니다.
    for (Node* node : nodes)
        delete node;
}

void AStar::Render()
{
    // 그리드 내의 각 노드를 렌더링합니다.
    for (Node* node : nodes)
        node->Render();
}

void AStar::SetNode(Terrain* terrain)
{
    // 지형 크기를 기반으로 노드 간의 간격을 계산합니다.
    Float2 size = terrain->GetSize();
    interval.x = size.x / width;
    interval.y = size.y / height;

    // 노드의 자주 발생하는 재할당을 피하기 위해 공간을 예약합니다.
    nodes.reserve((width + 1) * (height + 1));

    // 각 그리드 위치에 대한 노드를 생성합니다.
    for (UINT z = 0; z <= height; z++)
    {
        for (UINT x = 0; x <= width; x++)
        {
            Vector3 pos = Vector3(x * interval.x, 0, z * interval.y);
            pos.y = terrain->GetOnGrondPosition(pos).y;

            // 노드를 생성하고 목록에 추가합니다.
            nodes.push_back(new Node(pos, nodes.size()));
            nodes.back()->UpdateWorld();

            // 지형 높이가 일정 값 이상인 경우 장애물로 설정합니다.
            if (pos.y > 5.0f)
            {
                nodes.back()->SetState(Node::OBSTACLE);
            }
        }
    }

    // 노드 간의 엣지를 설정합니다.
    SetEdge();
}

int AStar::FindCloseNode(Vector3 pos)
{
    // TODO: 주어진 위치에 가장 가까운 노드를 찾는 로직을 구현합니다.
    return 0;
}

void AStar::GetPath(IN int start, IN int end, OUT vector<Vector3>& path)
{
    // TODO: 시작 노드에서 종단 노드까지의 최적 경로를 찾는 로직을 구현합니다.
}

bool AStar::IsCollisionObstacle(Vector3 start, Vector3 end)
{
    // TODO: 두 지점 사이에 장애물과 충돌이 있는지 확인하는 로직을 구현합니다.
    return false;
}

void AStar::Reset()
{
    // TODO: A* 알고리즘의 내부 상태를 초기화하는 로직을 구현합니다.
}

float AStar::GetDiagonalManhattanDistance(int start, int end)
{
    // TODO: 두 노드 사이의 대각 맨해튼 거리를 계산하는 로직을 구현합니다.
    return 0.0f;
}

void AStar::Extend(int center, int end)
{
    // TODO: 현재 중심 노드에서 종단 노드로 검색을 확장하는 로직을 구현합니다.
}

int AStar::GetMinNode()
{
    // TODO: openNodes 목록에서 총 비용이 가장 작은 노드를 찾는 로직을 구현합니다.
    return 0;
}

void AStar::SetEdge()
{
    // 그리드 내의 각 노드에 대해 엣지를 설정합니다.
    UINT gridWidth = this->width + 1;

    FOR(nodes.size())
    {
        if (i % gridWidth != gridWidth - 1)
        {
            // 오른쪽 노드와의 엣지를 추가합니다.
            nodes[i]->AddEdge(nodes[i + 1]);
            nodes[i + 1]->AddEdge(nodes[i]);
        }

        if (i < nodes.size() - gridWidth)
        {
            // 아래쪽 노드와의 엣지를 추가합니다.
            nodes[i]->AddEdge(nodes[i + gridWidth]);
            nodes[i + gridWidth]->AddEdge(nodes[i]);
        }

        if (i % gridWidth != gridWidth - 1 && i < nodes.size() - gridWidth)
        {
            // 오른쪽 아래 대각선 노드와의 엣지를 추가합니다.
            nodes[i]->AddEdge(nodes[i + 1 + gridWidth]);
            nodes[i + 1 + gridWidth]->AddEdge(nodes[i]);
        }

        if (i % gridWidth != 0 && i < nodes.size() - gridWidth)
        {
            // 왼쪽 아래 대각선 노드와의 엣지를 추가합니다.
            nodes[i]->AddEdge(nodes[i - 1 + gridWidth]);
            nodes[i - 1 + gridWidth]->AddEdge(nodes[i]);
        }
    }
}