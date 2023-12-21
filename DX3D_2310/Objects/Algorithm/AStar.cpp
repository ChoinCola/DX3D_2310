#include "Framework.h"

AStar::AStar(UINT width, UINT height)
    : width(width), height(height)
{
    heap = new Heap();
}

AStar::~AStar()
{
    // �������� �Ҵ�� ������ �����մϴ�.
    for (Node* node : nodes)
        delete node;

    delete heap;
}

void AStar::Render()
{
    // �׸��� ���� �� ��带 �������մϴ�.
    for (Node* node : nodes)
        node->Render();
}

void AStar::SetNode(Terrain* terrain)
{
    // ���� ũ�⸦ ������� ��� ���� ������ ����մϴ�.
    Float2 size = terrain->GetSize();

    interval.x = size.x / width;
    interval.y = size.y / height;

    // ����� ���� �߻��ϴ� ���Ҵ��� ���ϱ� ���� ������ �����մϴ�.
    nodes.reserve((width + 1) * (height + 1));

    // �� �׸��� ��ġ�� ���� ��带 �����մϴ�.
    for (UINT z = 0; z <= height; z++)
    {
        for (UINT x = 0; x <= width; x++)
        {
            Vector3 pos = Vector3(x * interval.x, 0, z * interval.y);
            pos.y = terrain->GetOnGrondPosition(pos).y;

            // ��带 �����ϰ� ��Ͽ� �߰��մϴ�.
            nodes.push_back(new Node(pos, nodes.size()));
            nodes.back()->UpdateWorld();

            // ���� ���̰� ���� �� �̻��� ��� ��ֹ��� �����մϴ�.
            if (pos.y > 5.0f)
            {
                nodes.back()->SetState(Node::OBSTACLE);
            }
        }
    }

    // ��� ���� ������ �����մϴ�.
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

    //1. ���۳�� �ʱ�ȭ�ϰ� ���³�忡 �߰�
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
        //���� ���� ��Ȳ
        //if (openNodes.empty())
        if (heap->Empty())
            return;

        //2. ���³�� �߿��� ȿ���� ���� ���� ��� ã��
        int curIndex = GetMinNode();
        //3. ã�� ���� ����� ����� ������ ���� �� ���³�忡 �߰��ϰ�
        //Ȯ���� ���� ���� �ݱ�
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
    //���۳�� �߰��ϱ�
    //path.push_back(nodes[start]->GetGlobalPosition());
}

bool AStar::IsCollisionObstacle(Vector3 start, Vector3 end)
{
    // TODO: �� ���� ���̿� ��ֹ��� �浹�� �ִ��� Ȯ���ϴ� ������ �����մϴ�.
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