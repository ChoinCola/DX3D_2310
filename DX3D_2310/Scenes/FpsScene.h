#pragma once
class FpsScene : public Scene
{
	FpsScene();
	~FpsScene();

	// Scene��(��) ���� ��ӵ�
	void Update() override;
	void PreRender() override;
	void Render() override;
	void PostRender() override;
	void GUIRender() override;
};