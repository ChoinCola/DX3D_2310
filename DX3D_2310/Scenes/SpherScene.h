#pragma once
class SpherScene : public Scene
{
public:
	SpherScene();
	~SpherScene();

	// Scene��(��) ���� ��ӵ�
	void Update() override;
	void PreRender() override;
	void Render() override;
	void PostRender() override;
	void GUIRender() override;
private:
	Spher* spher = nullptr;
};