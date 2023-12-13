#include "Framework.h"
#include "UnitCollider.h"

UnitCollider::UnitCollider(Transform* model) 
	: inputmodel(model)
{
}

UnitCollider::~UnitCollider()
{
}

void UnitCollider::Update()
{
}

void UnitCollider::Render()
{
	__super::Render();
}

void UnitCollider::ClickObject(bool input)
{
	inputmodel->SetPreRender(input);
}
