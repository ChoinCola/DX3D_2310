#include "Framework.h"

Block::Block(BlockData data)
	: data(data)
{
	SetTag(data.name + "Collider");
	model = new Model(data.modelname);
	model->SetParent(this);
	model->SetLocalScale(Vector3(0.1, 0.1, 0.1));
	model->SetLocalPosition(GetLocalPosition() - Vector3(0, +0.5, 0));

	string insertname = "Textures/UI/Blocks/" + data.modelname + ".png";
	string Test = "Textures/Colors/Black.png";
	Inventorymodel = new Button(ToWString(insertname));
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
	if (CAM->ContainSphere(localPosition, 0.5) == false)
		return;

	model->Render();
	//Collider::Render();
}

void Block::InventoryRender()
{
	Inventorymodel->UpdateWorld();
	Inventorymodel->Render();
}

void Block::PostRender()
{
	if (CAM->ContainSphere(localPosition, 0.5f) == false)
		return;

	Vector3 screenPos = CAM->WorldToScreen(localPosition + Vector3(0, 0.5f, 0));
	Font::Get()->RenderText(data.name, { screenPos.x, screenPos.y });

}

void Block::GUIRender()
{
	if (CAM->ContainSphere(localPosition, 0.5f) == false)
		return;
	model->GUIRender();
	Transform::GUIRender();
}

Vector3 Block::GetDirection(Vector3 point)
{
	// 가져온 point와 가장 가까운 육면체의 면 한개를 반환하는 함수이다.
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
		// 모든 위치에서 내적한 값을 가져온다.
		float dot = Vector3::Dot(plane, direction);
		float angle = abs(acos(dot));
		// 내적위치의 각도가 최저각도와 비교해서 작을경우 갱신함.

		if (angle < minAngle)
		{
			minAngle = angle;
			result = plane;
		}
	}

	return result;
}
