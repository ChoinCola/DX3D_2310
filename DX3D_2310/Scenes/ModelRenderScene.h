#pragma once

class ModelRenderScene : public Scene
{
public:
	ModelRenderScene();
	~ModelRenderScene();

	// Scene��(��) ���� ��ӵ�
	void Update() override;
	void PreRender() override;
	void Render() override;
	void PostRender() override;
	void GUIRender() override;

private:
	Model* model;
};