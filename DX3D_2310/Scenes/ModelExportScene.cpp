#include "Framework.h"
#include "ModelExportScene.h"

ModelExportScene::ModelExportScene()
{
	string name = "Jin";
	string file = "Models/FBX/" + name + ".fbx";

	exporter = new ModelExporter(name, file);
	exporter->ExportMaterial();

	delete exporter;
}

ModelExportScene::~ModelExportScene()
{
}

void ModelExportScene::Update()
{

}

void ModelExportScene::PreRender()
{

}

void ModelExportScene::Render()
{

}

void ModelExportScene::PostRender()
{

}

void ModelExportScene::GUIRender()
{

}
