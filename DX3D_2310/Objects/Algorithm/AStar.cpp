#include "Framework.h"

AStar::AStar(UINT width, UINT height)
    : width(width), height(height)
{
}

AStar::~AStar()
{
    // �������� �Ҵ�� ������ �����մϴ�.
    for (Node* node : nodes)
        delete node;
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
    // TODO: �־��� ��ġ�� ���� ����� ��带 ã�� ������ �����մϴ�.
    return 0;
}

void AStar::GetPath(IN int start, IN int end, OUT vector<Vector3>& path)
{
    // TODO: ���� ��忡�� ���� �������� ���� ��θ� ã�� ������ �����մϴ�.
}

bool AStar::IsCollisionObstacle(Vector3 start, Vector3 end)
{
    // TODO: �� ���� ���̿� ��ֹ��� �浹�� �ִ��� Ȯ���ϴ� ������ �����մϴ�.
    return false;
}

void AStar::Reset()
{
    // TODO: A* �˰����� ���� ���¸� �ʱ�ȭ�ϴ� ������ �����մϴ�.
}

float AStar::GetDiagonalManhattanDistance(int start, int end)
{
    // TODO: �� ��� ������ �밢 ����ư �Ÿ��� ����ϴ� ������ �����մϴ�.
    return 0.0f;
}

void AStar::Extend(int center, int end)
{
    // TODO: ���� �߽� ��忡�� ���� ���� �˻��� Ȯ���ϴ� ������ �����մϴ�.
}

int AStar::GetMinNode()
{
    // TODO: openNodes ��Ͽ��� �� ����� ���� ���� ��带 ã�� ������ �����մϴ�.
    return 0;
}

void AStar::SetEdge()
{
    // �׸��� ���� �� ��忡 ���� ������ �����մϴ�.
    UINT gridWidth = this->width + 1;

    FOR(nodes.size())
    {
        if (i % gridWidth != gridWidth - 1)
        {
            // ������ ������ ������ �߰��մϴ�.
            nodes[i]->AddEdge(nodes[i + 1]);
            nodes[i + 1]->AddEdge(nodes[i]);
        }

        if (i < nodes.size() - gridWidth)
        {
            // �Ʒ��� ������ ������ �߰��մϴ�.
            nodes[i]->AddEdge(nodes[i + gridWidth]);
            nodes[i + gridWidth]->AddEdge(nodes[i]);
        }

        if (i % gridWidth != gridWidth - 1 && i < nodes.size() - gridWidth)
        {
            // ������ �Ʒ� �밢�� ������ ������ �߰��մϴ�.
            nodes[i]->AddEdge(nodes[i + 1 + gridWidth]);
            nodes[i + 1 + gridWidth]->AddEdge(nodes[i]);
        }

        if (i % gridWidth != 0 && i < nodes.size() - gridWidth)
        {
            // ���� �Ʒ� �밢�� ������ ������ �߰��մϴ�.
            nodes[i]->AddEdge(nodes[i - 1 + gridWidth]);
            nodes[i - 1 + gridWidth]->AddEdge(nodes[i]);
        }
    }
}