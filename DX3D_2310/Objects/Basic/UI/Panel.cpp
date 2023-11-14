#include "Framework.h"
#include "Panel.h"

Panel::Panel(wstring texturefile)
	:	Quad(texturefile)
{
	startIcon = new Transform();
	startIcon->SetTag("StartIcon");
	startIcon->SetParent(this);
	startIcon->Load();
}

Panel::~Panel()
{
	Save();
}

void Panel::Update()
{
	if (MouseBag::Get()->GetBlock() == nullptr) {
		if (!isDrag && Mouse::Get()->Down(0) && CollisionChack(Mouse::Get()->GetPosition()))
		{
			isDrag = true;
			dragOffset = GetGlobalPosition() - Mouse::Get()->GetPosition();
		}

		if (isDrag && Mouse::Get()->Press(0))
		{
			SetLocalPosition(Mouse::Get()->GetPosition() + dragOffset);
		}
	}
	if (Mouse::Get()->Up(0))
		isDrag = false;

	UpdateWorld();
	startIcon->UpdateWorld();
}

void Panel::GUIRender()
{

}
