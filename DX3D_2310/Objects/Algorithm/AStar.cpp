#include "Framework.h"

AStar::AStar(UINT width, UINT height)
    : width(width), height(height)
{
}

AStar::~AStar()
{
    for (Node* node : nodes)
        delete node;
}

void AStar::Render()
{
    for (Node* node : nodes)
        node->Render();
}

void AStar::SetNode(Terrain* terrain)
{
    Float2 size = terrain->GetSize();

    interval.x = size.x / width;
    interval.y = size.y / height;

    nodes.reserve((width + 1) * (height + 1));
    for (UINT z = 0; z <= height; z++)
    {
        for (UINT x = 0; x <= width; x++)
        {
            Vector3 pos = Vector3(x * interval.x, 0, z * interval.y);
            pos.y = terrain->GetOnGrondPosition(pos).y;

            nodes.push_back(new Node(pos, nodes.size()));
            nodes.back()->UpdateWorld();
            if (pos.y > 5.0f)
            {
                nodes.back()->SetState(Node::OBSTACLE);
            }
        }
    }

    SetEdge();
}

int AStar::FindCloseNode(Vector3 pos)
{
    return 0;
}

void AStar::GetPath(IN int start, IN int end, OUT vector<Vector3>& path)
{
}

bool AStar::IsCollisionObstacle(Vector3 start, Vector3 end)
{
    return false;
}

void AStar::Reset()
{
}

float AStar::GetDiagonalManhattanDistance(int start, int end)
{
    return 0.0f;
}

void AStar::Extend(int center, int end)
{
}

int AStar::GetMinNode()
{
    return 0;
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
