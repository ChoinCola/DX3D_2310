#pragma once

// Button Ŭ������ ����ϴ� InvenBlock Ŭ������ �����մϴ�.
class InvenBlock : public Button
{
public:
	// InvenBlock�� ������ �Լ� ����
	InvenBlock(Vector3 pos, Transform* Parent = nullptr);

	// InvenBlock�� �Ҹ��� �Լ� ����
	~InvenBlock() = default;

	// InvenBlock�� �ʱ�ȭ�ϴ� �Լ� ����
	void clear();

	// InvenBlock�� ������Ʈ�ϴ� �Լ� ����
	void Update();

	// InvenBlock�� ȭ�鿡 �������ϴ� �Լ� ����
	void PostRender();

	void ShowItemHud();
	void IsShowItemHud(int input);
	// ����� InvenBlock�� �����ϴ� �Լ� ����
	bool InsertBlock(Block* block, UINT count = 1, bool property = true);

	// InvenBlock�� �ٸ� InvenBlock�� �����ϴ� �Լ� ����
	bool InsertBlock(InvenBlock* block);

	// InvenBlock���� ����� ������ �Լ� ����
	Block* PopBlock();

	// �ٸ� InvenBlock�� ����� ���ϴ� �Լ� ����
	bool CheckBlock(InvenBlock* block);

	// ���콺���� ����� ������ �Լ� ����
	void PopMouse();
	// ���콺�� ����� �����ϴ� �Լ� ����
	void InsertMouseFrominven();

	// ����� ������ ��ȯ�ϴ� �Լ� ����
	const UINT& GetBlockCount() { return count; }

	// ���� InvenBlock�� ����ִ� ����� ��ȯ�ϴ� �Լ� ����
	Block* GetBlock() { return block; }

	// InvenBlock�� ����ִ� ��� ������ ��ȯ�ϴ� �Լ� ����
	UINT GetCount() { return count; }

	// InvenBlock�� ����� �����ϴ� �Լ� ����
	void SetBlock(Block* input) { block = input; }

	// InvenBlock�� ����ִ� ��� ������ �����ϴ� �Լ� ����
	void SetCount(UINT input) { count = input; }

	void SetHasPlayer() { hasPlayer = !hasPlayer; }
	void SetCanInput() { canInput = !canInput; }
	void SetCanPop() { canPop = !canPop; }

	bool GetHasPlayer() { return hasPlayer; }
private:
	UINT count = 0; // InvenBlock�� ����ִ� ����� ����
	Vector3 mainPos; // InvenBlock�� ��ġ
	Block* block = nullptr; // InvenBlock�� ����ִ� ����� ������
	Quad* hashud = nullptr;
	Quad* ShowDetails = nullptr;

	bool hasPlayer = true;
	bool canInput = true;
	bool canPop = true;
};