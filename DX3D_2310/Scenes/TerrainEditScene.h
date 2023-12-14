#pragma once

class TerrainEditScene : public Scene
{
public:
	TerrainEditScene();
	~TerrainEditScene();

	// Scene을(를) 통해 상속됨
	void Update() override;
	void PreRender() override;
	void Render() override;
	void PostRender() override;
	void GUIRender() override;

private:
	void ByteAddress();

private:

	TerrainEditer* terrainEditor;
};