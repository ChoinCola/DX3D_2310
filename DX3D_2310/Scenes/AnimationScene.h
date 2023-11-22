#pragma once

class AnimationScene : public Scene
{
public:
	AnimationScene();
	~AnimationScene();

	// Scene을(를) 통해 상속됨
	void Update() override;
	void PreRender() override;
	void Render() override;
	void PostRender() override;
	void GUIRender() override;

	void End();
private:
	ModelAnimator* model;
	Model* weapon;

	Transform* rightHand;
};