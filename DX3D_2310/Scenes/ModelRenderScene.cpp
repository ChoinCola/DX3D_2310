#include "Framework.h"
#include "ModelRenderScene.h"

ModelRenderScene::ModelRenderScene()
{
	string name = "Box";

	model = new Model(name);
}

ModelRenderScene::~ModelRenderScene()
{

}

void ModelRenderScene::Update()
{
	model->UpdateWorld();
}

void ModelRenderScene::PreRender()
{
}

void ModelRenderScene::Render()
{
	model->Render();
}

void ModelRenderScene::PostRender()
{
}

void ModelRenderScene::GUIRender()
{
	model->GUIRender();
}
