#include "Framework.h"
#include "InvenBlock.h"

// InvenBlock Ŭ������ ������ �Լ� ����
InvenBlock::InvenBlock(Vector3 pos, Transform* Parent)
	: Button(Float2(16, 16)), mainPos(pos)
{
	// Button Ŭ������ �����ڸ� ȣ���ϰ� ��ư ũ��� ��ġ�� �����մϴ�.
	GetMaterial()->SetDiffuseMap(L"Textures/Colors/Blue.png"); // ��ư�� ��ǻ�� �� ����
	//hashud = new Quad(L"Textures/Colors/Blue.png");
	hashud = new Quad(L"Textures/UI/MineCraftUI/NonHasPlayerObject.png");
	hashud->SetParent(this);

	ShowDetails = new Quad(Vector3(100, 100));
	ShowDetails->SetParent(this);
	ShowDetails->SetLocalPosition(Vector3(50, 50));

	Observer::Get()->AddIntParamEvent("SetHudEvent", bind(&InvenBlock::IsShowItemHud, this, placeholders::_1));
	SetLocalPosition(pos); // ��ư�� ���� ��ġ ����

	if (Parent != nullptr)
		SetParent(Parent); // �θ� Transform ����
}

// InvenBlock�� �ʱ�ȭ�ϴ� �Լ� ����
void InvenBlock::clear()
{
	count = 0;
	block = nullptr;

	if (GetRender() == true)
		IsRender();
}

// InvenBlock�� ������Ʈ�ϴ� �Լ� ����
void InvenBlock::Update()
{
	__super::Update(); // ���� Ŭ����(Button)�� Update �Լ� ȣ��

	if (block != nullptr)
		block->Update(); // InvenBlock�� ����ִ� ����� ������Ʈ

	UpdateWorld(); // ���� ��ȯ ������Ʈ

	if (hasPlayer)
		hashud->GetMaterial()->SetDiffuseMap(L"Textures/UI/MineCraftUI/HasPlayerObject.png");
	else
		hashud->GetMaterial()->SetDiffuseMap(L"Textures/UI/MineCraftUI/NonHasPlayerObject.png");

	hashud->UpdateWorld();
	ShowDetails->UpdateWorld();
}

// ����� InvenBlock�� �����ϴ� �Լ� ����
bool InvenBlock::InsertBlock(Block* block, UINT count, bool property)
{
	// ����� ������ Ȯ��. ���� �������� �����۰� �̵��Ϸ��� ĭ�� ������쿡�� �̵��� ����.
	if (block != nullptr && this->block == nullptr && (property == hasPlayer || property)) {
		wstring name = ToWString(block->GetBlockData().modelname) + L".png";
		GetMaterial()->SetDiffuseMap(L"Textures/UI/Blocks/" + name); // ��ư�� ��ǻ�� ���� ��� �ؽ�ó�� ����

		if (GetRender() == false)
			IsRender();

		this->block = block;
		this->count = count;
		this->hasPlayer = property;
		return true;
	}

	// �̰Ͷ��� ����������.
	if (block != nullptr && (this->hasPlayer == property) &&
		(block->GetBlockData().name == this->block->GetBlockData().name)) {
		delete block;
		this->count += count;
		return true;
	}
	return false;
}

// InvenBlock�� �ٸ� InvenBlock�� �����ϴ� �Լ� ����
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

// InvenBlock���� ����� ������ �Լ� ����
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

// �ٸ� InvenBlock�� ����� ���ϴ� �Լ� ����
bool InvenBlock::CheckBlock(InvenBlock* block)
{
	if (block->GetBlock()->GetBlockData().name == GetBlock()->GetBlockData().name)
		return true;
	return false;
}

// InvenBlock�� ȭ�鿡 �������ϴ� �Լ� ����
void InvenBlock::PostRender()
{
	if (block != nullptr)
	{
		string c = to_string(count);
		Vector3 pos = GetGlobalPosition();

		Font::Get()->RenderText(c, Float2(pos.x + 18, pos.y - 8)); // ��� ������ ȭ�鿡 ǥ��
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

		Font::Get()->RenderText("������ : ", pos + Vector3(-10, -60, 0));
		Font::Get()->RenderText(havenow, pos + Vector3(100, -60, 0));
	}
}

void InvenBlock::IsShowItemHud(int input)
{
	ChackHudprint = input;
}

// ���콺���� ����� ������ �Լ� ����
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

// ����� �����ϴ� �Լ� ����
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