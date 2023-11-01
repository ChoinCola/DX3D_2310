#include "Framework.h"
#include "LightScene.h"

LightScene::LightScene()
{
    coin = new Bullet(Vector3(0, 1, 0));
    terrain = new Terrain(L"Textures/HeightMaps/Miro.png", 2, true);
    //terrain->GetMaterial()->SetDiffuseMap(L"Textures/Landscape/Floor.png");
    //terrain->GetMaterial()->SetNormalMap(L"Textures/Landscape/Floor_normal.png");
    quad = new Quad(Float2(1, 1));
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
    coin->Update();
    quad->UpdateWorld();
    //sphere->UpdateWorld();
}

void LightScene::PreRender()
{
}

void LightScene::Render()
{
    coin->Render();
    //terrain->Render();
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
    //sphere->GUIRender();
    //sphere->GetMaterial()->GUIRender();
}
