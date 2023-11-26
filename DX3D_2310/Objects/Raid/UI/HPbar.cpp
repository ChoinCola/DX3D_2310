#include "Framework.h"
#include "HPbar.h"

HPbar::HPbar(string input, Transform* parent): name(input)
{
	UIRenderMaster::Get()->InputUI3D(input, L"Textures/Colors/Red.png");
	bar = UIRenderMaster::Get()->GetUI3D(input);
	bar->SetParent(parent);
}

HPbar::~HPbar()
{
	delete bar;
}

void HPbar::UpdateHPbar(float input, float inputmax)
{
	UIRenderMaster::Get()->Set3DUIScaleAtSort
	(name, 0, input / inputmax, 1.0f, Vector3(0, 1.5, 0), Vector3(0.01, 0.001, 0));
}
