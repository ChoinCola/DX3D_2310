#pragma once
class TutorialScene : public Scene
{
public:
	TutorialScene();
	~TutorialScene();

	// Scene을(를) 통해 상속됨
	void Update() override;
	void PreRneder() override;
	void Render() override;
	void PostRender() override;
	void GUIRender() override;

private:
	Cube* cube;
	Spher* ObjectTest;

	BaseLine* baseline;

	Moon* moon;
	Ealth* ealth;
	Sun* sun;

	Robot* robot;

	MatrixBuffer* viewBuffer;
	MatrixBuffer* projectionBuffer;
};