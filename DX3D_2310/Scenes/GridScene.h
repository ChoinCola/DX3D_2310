#pragma once

class GridScene : public Scene
{
public:
	GridScene();
	~GridScene();

	// Scene��(��) ���� ��ӵ�
	void Update() override;
	void PreRneder() override;
	void Render() override;
	void PostRender() override;
	void GUIRender() override;
private:

};