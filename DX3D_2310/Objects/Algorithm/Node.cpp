#include "Framework.h"

Node::Node(Vector3 pos, int index)
{
    // ����� ���� ��ġ�� �ʱ�ȭ�մϴ�.
    localPosition = pos;
}

Node::~Node()
{
    // �������� �Ҵ�� �������� �����մϴ�.
    for (Edge* edge : edges)
        delete edge;
}

void Node::Render()
{
    // ����� ���¿� ���� ������ �����մϴ�.
    switch (state)
    {
    case Node::NONE:
        SetColor(0, 1, 1); // �ϴû�
        break;
    case Node::OPEN:
        SetColor(0, 0, 1); // �Ķ���
        break;
    case Node::CLOSED:
        SetColor(0, 0, 0); // ������
        break;
    case Node::USING:
        SetColor(0, 1, 0); // ���
        break;
    case Node::OBSTACLE:
        SetColor(1, 0, 0); // ������
        break;
    }

    // Collider�� Render �Լ��� ȣ���Ͽ� ������ �����մϴ�.
    Collider::Render();
}

void Node::AddEdge(Node* node)
{
    // ���ο� ������ �����ϰ� ������ �����մϴ�.
    Edge* edge = new Edge();
    edge->index = node->index;
    edge->cost = MATH->Distance(node->GetGlobalPosition(), GetGlobalPosition());

    // ������ ����� ���� ��Ͽ� �߰��մϴ�.
    edges.push_back(edge);
}