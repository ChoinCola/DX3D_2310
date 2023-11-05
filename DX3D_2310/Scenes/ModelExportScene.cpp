#include "Framework.h"

ModelExportScene::ModelExportScene()
{
	string name = "Table";
	string file = "Models/FBX/" + name + ".fbx";

	exporter = new ModelExporter(name, file);
	exporter->ExportMaterial();
	exporter->ExportMesh();

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
