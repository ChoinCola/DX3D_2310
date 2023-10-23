#include "Framework.h"
#include "SpherScene.h"

SpherScene::SpherScene()
{
	spher = new Spher(2);
	spher->SetTag("Spher1");
}

SpherScene::~SpherScene()
{
	delete spher;
}

void SpherScene::Update()
{
	spher->Update();
	spher->UpdateWorld();
}

void SpherScene::PreRender()
{
}

void SpherScene::Render()
{
	spher->Render();

}

void SpherScene::PostRender()
{
}

void SpherScene::GUIRender()
{
	spher->GUIRender();
}
