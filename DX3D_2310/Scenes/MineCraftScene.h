#pragma once

class MinceCraftScene : public Scene
{
public:
	MinceCraftScene();
	~MinceCraftScene();

	// Scene��(��) ���� ��ӵ�
	void Update() override;
	void PreRender() override;
	void Render() override;
	void PostRender() override;
	void GUIRender() override;
};