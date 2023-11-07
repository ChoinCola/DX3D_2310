#include "Framework.h"

Block::Block(string name)
{
	SetTag(name + "Collider");
	model = new Model(name);
	model->SetParent(this);
	model->SetLocalScale(Vector3(0.1, 0.1, 0.1));
	model->SetLocalPosition(GetLocalPosition() - Vector3(0, +0.5, 0));

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

void Block::GUIRender()
{
	model->GUIRender();
	Transform::GUIRender();
}
