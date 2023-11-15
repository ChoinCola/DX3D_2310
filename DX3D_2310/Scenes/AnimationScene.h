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

private:
	Model* model;
	Model* weapon;

	Transform* rightHand;
};