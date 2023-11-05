#include "Framework.h"
#include "Item.h"


Item::Item(string Id, UINT* numberpointer)
	: ID(Id), Objectnumber(numberpointer)
{

}

Item::~Item()
{
	Save();
}

void Item::InsertItem()
{

}

void Item::OutItem()
{

}

void Item::Update()
{
	UpdateWorld();
	Object->SetLocalPosition(GetLocalPosition());
	Object->UpdateWorld();
}

void Item::Render()
{
	if(IsRender)
		Object->Render();
}

void Item::GUIRender()
{
	Object->GUIRender();
	__super::GUIRender();
}

void Item::Load()
{
	__super::Load();
}

void Item::Save()
{
	__super::Save();
}
