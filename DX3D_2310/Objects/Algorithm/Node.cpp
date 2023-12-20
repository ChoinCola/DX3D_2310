#include "Framework.h"

Node::Node(Vector3 pos, int index)
{
    // 노드의 로컬 위치를 초기화합니다.
    localPosition = pos;
}

Node::~Node()
{
    // 동적으로 할당된 엣지들을 정리합니다.
    for (Edge* edge : edges)
        delete edge;
}

void Node::Render()
{
    // 노드의 상태에 따라 색상을 설정합니다.
    switch (state)
    {
    case Node::NONE:
        SetColor(0, 1, 1); // 하늘색
        break;
    case Node::OPEN:
        SetColor(0, 0, 1); // 파란색
        break;
    case Node::CLOSED:
        SetColor(0, 0, 0); // 검은색
        break;
    case Node::USING:
        SetColor(0, 1, 0); // 녹색
        break;
    case Node::OBSTACLE:
        SetColor(1, 0, 0); // 빨간색
        break;
    }

    // Collider의 Render 함수를 호출하여 색상을 적용합니다.
    Collider::Render();
}

void Node::AddEdge(Node* node)
{
    // 새로운 엣지를 생성하고 정보를 설정합니다.
    Edge* edge = new Edge();
    edge->index = node->index;
    edge->cost = MATH->Distance(node->GetGlobalPosition(), GetGlobalPosition());

    // 엣지를 노드의 엣지 목록에 추가합니다.
    edges.push_back(edge);
}