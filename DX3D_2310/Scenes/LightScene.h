#pragma once
class LightScene : public Scene
{
public:
	LightScene();
	~LightScene();


	// Scene��(��) ���� ��ӵ�
	void Update() override;
	void PreRender() override;
	void Render() override;
	void PostRender() override;
	void GUIRender() override;
private:
	Item* coin;
	Terrain* terrain;
	Quad* quad;
	Sphere* sphere;
};