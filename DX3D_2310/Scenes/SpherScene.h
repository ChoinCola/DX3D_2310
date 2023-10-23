#pragma once
class SpherScene : public Scene
{
public:
	SpherScene();
	~SpherScene();

	// Scene을(를) 통해 상속됨
	void Update() override;
	void PreRender() override;
	void Render() override;
	void PostRender() override;
	void GUIRender() override;
private:
	Spher* spher = nullptr;
};