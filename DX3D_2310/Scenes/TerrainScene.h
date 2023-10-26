#pragma once

class TerrainScene : public Scene
{
public:
	TerrainScene();
	~TerrainScene();

	// Scene을(를) 통해 상속됨
	void Update() override;
	void PreRender() override;
	void Render() override;
	void PostRender() override;
	void GUIRender() override;
private:
	Robot* robot;
	Terrain* terrain;
	Sphere* sphere;
};