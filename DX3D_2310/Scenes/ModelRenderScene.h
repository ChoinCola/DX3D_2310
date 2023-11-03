#pragma once

class ModelRenderScene : public Scene
{
public:
	ModelRenderScene();
	~ModelRenderScene();

	// Scene을(를) 통해 상속됨
	void Update() override;
	void PreRender() override;
	void Render() override;
	void PostRender() override;
	void GUIRender() override;

private:
	Model* model;
};