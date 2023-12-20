#pragma once
class Node : public BoxCollider
{
public:
    friend class AStar;

    // ����� ���¸� �����ϴ� ������
    enum State
    {
        NONE, OPEN, CLOSED, USING, OBSTACLE
    };

    // ��� ���� ���� ������ �����ϴ� ����ü
    struct Edge
    {
        int index;  // ����� ����� �ε���
        float cost; // ������ ���
    };

public:
    // ������: ��ġ�� �ε����� �ʱ�ȭ�մϴ�.
    Node(Vector3 pos, int index);

    // �Ҹ���: �������� �Ҵ�� ������ �����մϴ�.
    ~Node();

    // ��带 �������ϴ� �Լ�
    void Render();

    // �ٸ� ������ ����� ������ �߰��ϴ� �Լ�
    void AddEdge(Node* node);

    // ����� ���¸� �����ϴ� �Լ�
    void SetState(State state) { this->state = state; }

    // ��������� �� ����� ��ȯ�ϴ� �Լ�
    float GetCost() { return f; }

private:
    int index = 0; // ����� �ε���
    int via = -1;  // ��� Ž�� �� ������ ����� �ε���

    // ���
    float f = 0, g = 0, h = 0;

    State state = NONE; // ����� ����

    vector<Edge*> edges; // ���� ����� �������� ���
};