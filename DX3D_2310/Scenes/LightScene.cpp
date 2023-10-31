#include "Framework.h"
#include "LightScene.h"

LightScene::LightScene()
{
    quad = new Quad();
    quad->Load();
    sphere = new Sphere();
    sphere->Load();
    //sphere->GetMaterial()->Load("TextData/Materials/Fieldstone.mat");
}

LightScene::~LightScene()
{
    delete quad;
    delete sphere;
}

void LightScene::Update()
{
    quad->UpdateWorld();
    sphere->UpdateWorld();
}

void LightScene::PreRender()
{
}

void LightScene::Render()
{
    quad->Render();
    //sphere->Render();
}

void LightScene::PostRender()
{
}

void LightScene::GUIRender()
{
    quad->GUIRender();
    quad->GetMaterial()->GUIRender();
    sphere->GUIRender();
    sphere->GetMaterial()->GUIRender();
}
