#include "Framework.h"

Sword::Sword() : Model("weapon0")
{
	collider = new BoxCollider();
	collider->SetTag(name + "Collider");
	collider->SetParent(this);
	collider->Load();
	collider->SetActive(false);
}

Sword::~Sword()
{
	delete collider;
}

void Sword::Update()
{
	UpdateWorld();
	collider->UpdateWorld();
}

void Sword::Render()
{
	Model::Render();
	collider->Render();
}
