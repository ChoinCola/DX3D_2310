#include "Framework.h"

AStar::AStar(UINT width, UINT height)
    : width(width), height(height)
{
    heap = new Heap();
}

AStar::~AStar()
{
    // 동적으로 할당된 노드들을 정리합니다.
    for (Node* node : nodes)
        delete node;

    delete heap;
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
    float minDist = FLT_MAX;
    int index = -1;

    FOR(nodes.size())
    {
        if (nodes[i]->state == Node::OBSTACLE)
            continue;

        float distance = MATH->Distance(pos, nodes[i]->GetGlobalPosition());

        if (minDist > distance)
        {
            minDist = distance;
            index = i;
        }
    }

    return index;
}

void AStar::GetPath(IN int start, IN int end, OUT vector<Vector3>& path)
{
    Reset();
    path.clear();

    //1. 시작노드 초기화하고 오픈노드에 추가
    float G = 0;
    float H = GetDiagonalManhattanDistance(start, end);

    nodes[start]->f = G + H;
    nodes[start]->g = G;
    nodes[start]->h = H;
    nodes[start]->via = start;
    nodes[start]->state = Node::OPEN;

    //openNodes.push_back(start);
    heap->Insert(nodes[start]);

    while (nodes[end]->state != Node::CLOSED)
    {
        //길이 막힌 상황
        //if (openNodes.empty())
        if (heap->Empty())
            return;

        //2. 오픈노드 중에서 효율이 가장 좋은 노드 찾기
        int curIndex = GetMinNode();
        //3. 찾은 노드와 연결된 노드의 정보를 갱신 후 오픈노드에 추가하고
        //확장이 끝난 노드는 닫기
        Extend(curIndex, end);
        nodes[curIndex]->state = Node::CLOSED;
    }

    //5. BackTracking
    int curIndex = end;

    while (curIndex != start)
    {
        nodes[curIndex]->state = Node::USING;
        path.push_back(nodes[curIndex]->GetGlobalPosition());
        curIndex = nodes[curIndex]->via;
    }
    //시작노드 추가하기
    //path.push_back(nodes[start]->GetGlobalPosition());
}

bool AStar::IsCollisionObstacle(Vector3 start, Vector3 end)
{
    // TODO: 두 지점 사이에 장애물과 충돌이 있는지 확인하는 로직을 구현합니다.
    return false;
}

void AStar::Reset()
{
    for (Node* node : nodes)
    {
        if (node->state != Node::OBSTACLE)
            node->SetState(Node::NONE);
    }

    //openNodes.clear();
    heap->Clear();
}

float AStar::GetDiagonalManhattanDistance(int start, int end)
{
    Vector3 startPos = nodes[start]->GetGlobalPosition();
    Vector3 endPos = nodes[end]->GetGlobalPosition();

    Vector3 temp = endPos - startPos;

    float x = abs(temp.x);
    float z = abs(temp.z);

    float minSize = min(x, z);
    float maxSize = max(x, z);

    return (maxSize - minSize) + sqrt(minSize * minSize * 2);
}

void AStar::Extend(int center, int end)
{
    for (Node::Edge* edge : nodes[center]->edges)
    {
        int index = edge->index;

        if (nodes[index]->state == Node::CLOSED)
            continue;
        if (nodes[index]->state == Node::OBSTACLE)
            continue;

        float G = nodes[center]->g + edge->cost;
        float H = GetDiagonalManhattanDistance(index, end);
        float F = G + H;

        if (nodes[index]->state == Node::OPEN)
        {
            if (F < nodes[index]->f)
            {
                nodes[index]->g = G;
                nodes[index]->f = F;
                nodes[index]->via = center;
            }
        }
        else if (nodes[index]->state == Node::NONE)
        {
            nodes[index]->g = G;
            nodes[index]->h = H;
            nodes[index]->f = F;
            nodes[index]->via = center;
            nodes[index]->state = Node::OPEN;

            //openNodes.push_back(index);
            heap->Insert(nodes[index]);
        }
    }
}

int AStar::GetMinNode()
{
    //int openIndex = 0;
    //int nodeIndex = openNodes[openIndex];
    //float minF = nodes[nodeIndex]->f;

    //FOR(openNodes.size())
    //{
    //    nodeIndex = openNodes[i];

    //    if (nodes[nodeIndex]->f < minF)
    //    {
    //        minF = nodes[nodeIndex]->f;
    //        openIndex = i;
    //    }
    //}
    //nodeIndex = openNodes[openIndex];
    //openNodes.erase(openNodes.begin() + openIndex);
    //return nodeIndex;
    return heap->DeleteRoot()->index;
}

void AStar::SetEdge()
{
    UINT width = this->width + 1;

    FOR(nodes.size())
    {
        if (i % width != width - 1)
        {
            nodes[i]->AddEdge(nodes[i + 1]);
            nodes[i + 1]->AddEdge(nodes[i]);
        }

        if (i < nodes.size() - width)
        {
            nodes[i]->AddEdge(nodes[i + width]);
            nodes[i + width]->AddEdge(nodes[i]);
        }

        if (i % width != width - 1 && i < nodes.size() - width)
        {
            nodes[i]->AddEdge(nodes[i + 1 + width]);
            nodes[i + 1 + width]->AddEdge(nodes[i]);
        }

        if (i % width != 0 && i < nodes.size() - width)
        {
            nodes[i]->AddEdge(nodes[i - 1 + width]);
            nodes[i - 1 + width]->AddEdge(nodes[i]);
        }
    }
}