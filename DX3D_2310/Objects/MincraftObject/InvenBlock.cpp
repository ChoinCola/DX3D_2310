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

}

// ����� InvenBlock�� �����ϴ� �Լ� ����
bool InvenBlock::InsertBlock(Block* block, UINT count, bool property)
{
	// ����� ������ Ȯ��. ���� �������� �����۰� �̵��Ϸ��� ĭ�� ������쿡�� �̵��� ����.
	if (block != nullptr && this->block == nullptr && (property == hasPlayer)) {
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
void InvenBlock::InsertBlock(InvenBlock* block)
{
	if (block != nullptr && CheckBlock(block)) {
		count += block->GetCount();
		delete block;
		return;
	}

	if (block != nullptr)
		block->SetLocalPosition(mainPos);
	return;
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
	}
	//if (GetRender() == false)
	//	IsRender();
	//Render();
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

// ���콺�� ����� �����ϴ� �Լ� ����
void InvenBlock::InsertMouseFrominven()
{
	if (!canInput) return;

	Block*& block = MouseBag::Get()->GetBlock();
	UINT& count = MouseBag::Get()->GetCount();
	InvenBlock*& Repos = MouseBag::Get()->GetBasePos();
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
	Repos->SetBlock(block);
	Repos->SetCount(count);

	count = 0;
	block = nullptr;
	MouseBag::Get()->SetHasPlayer(true);
}