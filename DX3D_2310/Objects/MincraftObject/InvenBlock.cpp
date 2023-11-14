#include "Framework.h"
#include "InvenBlock.h"

// InvenBlock 클래스의 생성자 함수 정의
InvenBlock::InvenBlock(Vector3 pos, Transform* Parent)
	: Button(Float2(16, 16)), mainPos(pos)
{
	// Button 클래스의 생성자를 호출하고 버튼 크기와 위치를 설정합니다.
	GetMaterial()->SetDiffuseMap(L"Textures/Colors/Blue.png"); // 버튼의 디퓨즈 맵 설정
	//hashud = new Quad(L"Textures/Colors/Blue.png");
	hashud = new Quad(L"Textures/UI/MineCraftUI/NonHasPlayerObject.png");
	hashud->SetParent(this);

	ShowDetails = new Quad(Vector3(100, 100));
	ShowDetails->SetParent(this);
	ShowDetails->SetLocalPosition(Vector3(50, 50));

	Observer::Get()->AddIntParamEvent("SetHudEvent", bind(&InvenBlock::IsShowItemHud, this, placeholders::_1));
	SetLocalPosition(pos); // 버튼의 로컬 위치 설정

	if (Parent != nullptr)
		SetParent(Parent); // 부모 Transform 설정
}

// InvenBlock를 초기화하는 함수 정의
void InvenBlock::clear()
{
	count = 0;
	block = nullptr;

	if (GetRender() == true)
		IsRender();
}

// InvenBlock을 업데이트하는 함수 정의
void InvenBlock::Update()
{
	__super::Update(); // 상위 클래스(Button)의 Update 함수 호출

	if (block != nullptr)
		block->Update(); // InvenBlock에 들어있는 블록을 업데이트

	UpdateWorld(); // 월드 변환 업데이트

	if (hasPlayer)
		hashud->GetMaterial()->SetDiffuseMap(L"Textures/UI/MineCraftUI/HasPlayerObject.png");
	else
		hashud->GetMaterial()->SetDiffuseMap(L"Textures/UI/MineCraftUI/NonHasPlayerObject.png");

	hashud->UpdateWorld();
	ShowDetails->UpdateWorld();
}

// 블록을 InvenBlock에 삽입하는 함수 정의
bool InvenBlock::InsertBlock(Block* block, UINT count, bool property)
{
	// 블록의 소유권 확인. 현재 소유권의 아이템과 이동하려는 칸이 같을경우에만 이동이 가능.
	if (block != nullptr && this->block == nullptr && (property == hasPlayer || property)) {
		wstring name = ToWString(block->GetBlockData().modelname) + L".png";
		GetMaterial()->SetDiffuseMap(L"Textures/UI/Blocks/" + name); // 버튼의 디퓨즈 맵을 블록 텍스처로 설정

		if (GetRender() == false)
			IsRender();

		this->block = block;
		this->count = count;
		this->hasPlayer = property;
		return true;
	}

	// 이것또한 마찬가지임.
	if (block != nullptr && (this->hasPlayer == property) &&
		(block->GetBlockData().name == this->block->GetBlockData().name)) {
		delete block;
		this->count += count;
		return true;
	}
	return false;
}

// InvenBlock에 다른 InvenBlock을 삽입하는 함수 정의
bool InvenBlock::InsertBlock(InvenBlock* block)
{
	if (block != nullptr && CheckBlock(block)) {
		count += block->GetCount();
		delete block;
		return true;
	}

	if (this->block == nullptr) {
		this->count = block->count;
		this->block = block->block;

		block->block = nullptr;
		block->count = 0;
		return true;
	}

	return false;
}

// InvenBlock에서 블록을 꺼내는 함수 정의
Block* InvenBlock::PopBlock()
{
	if (count == 1) {
		Block* result = block;
		clear();
		return result;
	}
	else
		count--;

	Block* result = nullptr;
	if (block != nullptr)
		result = new Block(block->GetBlockData());

	return result;
}

// 다른 InvenBlock과 블록을 비교하는 함수 정의
bool InvenBlock::CheckBlock(InvenBlock* block)
{
	if (block->GetBlock()->GetBlockData().name == GetBlock()->GetBlockData().name)
		return true;
	return false;
}

// InvenBlock를 화면에 렌더링하는 함수 정의
void InvenBlock::PostRender()
{
	if (block != nullptr)
	{
		string c = to_string(count);
		Vector3 pos = GetGlobalPosition();

		Font::Get()->RenderText(c, Float2(pos.x + 18, pos.y - 8)); // 블록 개수를 화면에 표시
		Render();
		hashud->Render();
		ShowItemHud();
	}
	//if (GetRender() == false)
	//	IsRender();
	//Render();
}

void InvenBlock::ShowItemHud()
{
	if (block == nullptr) return;

	if (CollisionChack(Mouse::Get()->GetPosition())) {
		BlockData data = block->GetBlockData();
		Vector3 pos = ShowDetails->GetGlobalPosition();
		string havenow;

		if (hasPlayer)
			havenow = "Player";
		else
			havenow = "NonPlayer";

		ShowDetails->Render();

		Font::Get()->RenderText("Name : ", pos + Vector3(-30, 90, 0));
		Font::Get()->RenderText(data.name, pos + Vector3(100, 90, 0));

		Font::Get()->RenderText("Damage : ", pos + Vector3(-10, 60, 0));
		Font::Get()->RenderText(to_string(data.damage), pos + Vector3(95, 60, 0));

		Font::Get()->RenderText("Cash : ", pos + Vector3(-30, 30, 0));
		Font::Get()->RenderText(to_string(data.cash), pos + Vector3(95, 30, 0));

		Font::Get()->RenderText("소유권 : ", pos + Vector3(-10, -60, 0));
		Font::Get()->RenderText(havenow, pos + Vector3(100, -60, 0));
	}
}

void InvenBlock::IsShowItemHud(int input)
{
	ChackHudprint = input;
}

// 마우스에서 블록을 꺼내는 함수 정의
void InvenBlock::PopMouse()
{
	if (!canPop) return;

	UINT& count = MouseBag::Get()->GetCount();
	InvenBlock*& Repos = MouseBag::Get()->GetBasePos();

	if (MouseBag::Get()->GetBlock() == nullptr && count <= 0)
	{
		Repos = this;
		count = this->count;
		this->count = 0;

		MouseBag::Get()->SetBlock(block);
		this->block = nullptr;

		MouseBag::Get()->SetHasPlayer(hasPlayer);
	}
}

// 블록을 삽입하는 함수 정의
void InvenBlock::InsertMouseFrominven()
{
	Block*& block = MouseBag::Get()->GetBlock();
	UINT& count = MouseBag::Get()->GetCount();
	InvenBlock*& Repos = MouseBag::Get()->GetBasePos();

	if (MouseBag::Get()->GetHasPlayer() || MouseBag::Get()->GetHasPlayer() == hasPlayer) {
		if (block == nullptr) return;

		if (this->block == nullptr || this->block->GetBlockData().key == block->GetBlockData().key) {
			if (InsertBlock(block, count, MouseBag::Get()->GetHasPlayer()))
			{
				Repos = nullptr;
				count = 0;
				block = nullptr;
				return;
			}
		}

	}

	Repos->SetBlock(block);
	Repos->SetCount(count);

	count = 0;
	block = nullptr;
	MouseBag::Get()->SetHasPlayer(true);
}