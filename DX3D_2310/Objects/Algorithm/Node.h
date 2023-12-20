#pragma once
class Node : public BoxCollider
{
public:
    friend class AStar;

    // 노드의 상태를 정의하는 열거형
    enum State
    {
        NONE, OPEN, CLOSED, USING, OBSTACLE
    };

    // 노드 간의 엣지 정보를 저장하는 구조체
    struct Edge
    {
        int index;  // 연결된 노드의 인덱스
        float cost; // 엣지의 비용
    };

public:
    // 생성자: 위치와 인덱스를 초기화합니다.
    Node(Vector3 pos, int index);

    // 소멸자: 동적으로 할당된 엣지를 정리합니다.
    ~Node();

    // 노드를 렌더링하는 함수
    void Render();

    // 다른 노드와의 연결된 엣지를 추가하는 함수
    void AddEdge(Node* node);

    // 노드의 상태를 설정하는 함수
    void SetState(State state) { this->state = state; }

    // 현재까지의 총 비용을 반환하는 함수
    float GetCost() { return f; }

private:
    int index = 0; // 노드의 인덱스
    int via = -1;  // 경로 탐색 시 경유한 노드의 인덱스

    // 비용
    float f = 0, g = 0, h = 0;

    State state = NONE; // 노드의 상태

    vector<Edge*> edges; // 노드와 연결된 엣지들의 목록
};