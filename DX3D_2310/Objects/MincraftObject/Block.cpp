#include "Framework.h"

Block::Block(string name)
{
	this->name = name;
	SetTag(name + "Collider");
	model = new Model(name);
	model->SetParent(this);
	model->SetLocalScale(Vector3(0.1, 0.1, 0.1));
	model->SetLocalPosition(GetLocalPosition() - Vector3(0, +0.5, 0));

	string insertname = "Textures/UI/Blocks/" + name + ".png";
	string Test = "Textures/Colors/Black.png";
	Inventorymodel = new Quad(ToWString(insertname));
}

Block::~Block()
{
	SAFE_DELETE(model);
}

void Block::Update()
{
	Transform::UpdateWorld();
	model->UpdateWorld();
}

void Block::Render()
{
	model->Render();
	//Collider::Render();
}

void Block::InventoryRender()
{
	Inventorymodel->UpdateWorld();
	Inventorymodel->Render();
}

void Block::GUIRender()
{
	model->GUIRender();
	Transform::GUIRender();
}

Vector3 Block::GetDirection(Vector3 point)
{
	// ������ point�� ���� ����� ����ü�� �� �Ѱ��� ��ȯ�ϴ� �Լ��̴�.
	Vector3 direction = (point - GetGlobalPosition()).GetNormalized();

	Vector3 plancs[6];
	plancs[0] = GetRight();
	plancs[1] = GetLeft();
	plancs[2] = GetUp();
	plancs[3] = GetDown();
	plancs[4] = GetForward();
	plancs[5] = GetBack();

	float minAngle = FLT_MAX;
	Vector3 result;

	for (Vector3 plane : plancs)
	{
		// ��� ��ġ���� ������ ���� �����´�.
		float dot = Vector3::Dot(plane, direction);
		float angle = abs(acos(dot));
		// ������ġ�� ������ ���������� ���ؼ� ������� ������.

		if (angle < minAngle)
		{
			minAngle = angle;
			result = plane;
		}
	}

	return result;
}
