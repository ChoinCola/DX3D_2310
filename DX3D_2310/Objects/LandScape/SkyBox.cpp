#include "Framework.h"

SkyBox::SkyBox(wstring textureFile)
{
	material->SetShader(L"Landscape/SkyBox.hlsl");

	cubeMap = Texture::Add(textureFile);

	rasterizerSate[0] = new RasterizerState();
	rasterizerSate[1] = new RasterizerState();
	rasterizerSate[1]->FrontCounterClockwise(true);

	depthState[0] = new DepthStencilState();
	depthState[1] = new DepthStencilState();
	depthState[1]->DepthEnable(false);
}

SkyBox::~SkyBox()
{
}

void SkyBox::Render()
{
	cubeMap->PSSet(10);
	rasterizerSate[1]->SetState();
	depthState[1]->SetState();

	Sphere::Render();
	rasterizerSate[0]->SetState();
	depthState[0]->SetState();

}
