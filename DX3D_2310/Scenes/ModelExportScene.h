#pragma once

class ModelExportScene : public Scene
{
public:
	ModelExportScene();
	~ModelExportScene();

	void Update() override;
	void PreRender() override;
	void Render() override;
	void PostRender() override;
	void GUIRender() override;

private:
	ModelExporter* exporter;
};