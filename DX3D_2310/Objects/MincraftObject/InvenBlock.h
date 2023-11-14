#pragma once

// Button 클래스를 상속하는 InvenBlock 클래스를 정의합니다.
class InvenBlock : public Button
{
public:
	// InvenBlock의 생성자 함수 정의
	InvenBlock(Vector3 pos, Transform* Parent = nullptr);

	// InvenBlock의 소멸자 함수 정의
	~InvenBlock() = default;

	// InvenBlock을 초기화하는 함수 정의
	void clear();

	// InvenBlock을 업데이트하는 함수 정의
	void Update();

	// InvenBlock을 화면에 렌더링하는 함수 정의
	void PostRender();

	void ShowItemHud();
	void IsShowItemHud(int input);
	// 블록을 InvenBlock에 삽입하는 함수 정의
	bool InsertBlock(Block* block, UINT count = 1, bool property = true);

	// InvenBlock에 다른 InvenBlock을 삽입하는 함수 정의
	bool InsertBlock(InvenBlock* block);

	// InvenBlock에서 블록을 꺼내는 함수 정의
	Block* PopBlock();

	// 다른 InvenBlock과 블록을 비교하는 함수 정의
	bool CheckBlock(InvenBlock* block);

	// 마우스에서 블록을 꺼내는 함수 정의
	void PopMouse();
	// 마우스에 블록을 삽입하는 함수 정의
	void InsertMouseFrominven();

	// 블록의 개수를 반환하는 함수 정의
	const UINT& GetBlockCount() { return count; }

	// 현재 InvenBlock에 들어있는 블록을 반환하는 함수 정의
	Block* GetBlock() { return block; }

	// InvenBlock에 들어있는 블록 개수를 반환하는 함수 정의
	UINT GetCount() { return count; }

	// InvenBlock에 블록을 설정하는 함수 정의
	void SetBlock(Block* input) { block = input; }

	// InvenBlock에 들어있는 블록 개수를 설정하는 함수 정의
	void SetCount(UINT input) { count = input; }

	void SetHasPlayer() { hasPlayer = !hasPlayer; }
	void SetCanInput() { canInput = !canInput; }
	void SetCanPop() { canPop = !canPop; }

	bool GetHasPlayer() { return hasPlayer; }
private:
	UINT count = 0; // InvenBlock에 들어있는 블록의 개수
	Vector3 mainPos; // InvenBlock의 위치
	Block* block = nullptr; // InvenBlock에 들어있는 블록의 포인터
	Quad* hashud = nullptr;
	Quad* ShowDetails = nullptr;

	bool hasPlayer = true;
	bool canInput = true;
	bool canPop = true;
};