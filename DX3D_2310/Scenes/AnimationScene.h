#pragma once

class AnimationScene : public Scene
{
public:
	AnimationScene();
	~AnimationScene();

	// Scene��(��) ���� ��ӵ�
	void Update() override;
	void PreRender() override;
	void Render() override;
	void PostRender() override;
	void GUIRender() override;

private:
	ModelAnimator* model;
};