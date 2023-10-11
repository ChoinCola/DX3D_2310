#pragma once
class TutorialScene : public Scene
{
public:
	TutorialScene();
	~TutorialScene();

	// Scene��(��) ���� ��ӵ�
	void Update() override;
	void PreRneder() override;
	void Render() override;
	void PostRender() override;
	void GUIRender() override;

private:
	vector<Cube*> cubes;

	MatrixBuffer* viewBuffer;
	MatrixBuffer* projectionBuffer;
};