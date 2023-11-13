#include "Framework.h"
#include "MouseBag.h"

void MouseBag::Render()
{
	if (block != nullptr)
	{
		if (mouseObject->GetRender() == false)
			mouseObject->IsRender();

		wstring def = ToWString(block->GetBlockData().modelname);
		mouseObject->GetMaterial()->SetDiffuseMap(L"Textures/UI/Blocks/" + def + L".png");

		string co = to_string(count);
		Float2 pos = mouseObject->GetLocalPosition() + Vector3(18, -8, 0);
		Font::Get()->RenderText(co, pos);
		mouseObject->Render();
	}
	else
	{
		if (mouseObject->GetRender() == true)
			mouseObject->IsRender();
	}
}

void MouseBag::Update()
{
	if (block != nullptr)
	{
		mouseObject->SetLocalPosition(Mouse::Get()->GetPosition());
		mouseObject->UpdateWorld();
	}
}
