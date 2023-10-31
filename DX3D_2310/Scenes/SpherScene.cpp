#include "Framework.h"
#include "SpherScene.h"

SpherScene::SpherScene()
{
	icosahedron = new Icosahedron(2);
	icosahedron->SetTag("Spher1");
	icosahedron->GetMaterial()->SetDiffuseMap(L"Textures/Landscape/Fieldstone_DM.tga");
}

SpherScene::~SpherScene()
{
	delete icosahedron;
}

void SpherScene::Update()
{
	icosahedron->Update();
	icosahedron->UpdateWorld();
}

void SpherScene::PreRender()
{
}

void SpherScene::Render()
{
	icosahedron->Render();

}

void SpherScene::PostRender()
{
}

void SpherScene::GUIRender()
{
	icosahedron->GUIRender();
}
