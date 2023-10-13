#pragma once

class GridScene : public Scene
{
public:
	GridScene();
	~GridScene();

	// Scene을(를) 통해 상속됨
	void Update() override;
	void PreRneder() override;
	void Render() override;
	void PostRender() override;
	void GUIRender() override;
private:

};