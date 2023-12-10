#include "Framework.h"

Sword::Sword() : Model("Sword")
{
	Load();
	collider = new BoxCollider();
	collider->SetTag(name + "Collider");
	collider->SetParent(this);
	collider->Load();
	collider->SetActive(false);
}

Sword::~Sword()
{
	Save();
	collider->Save();
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
