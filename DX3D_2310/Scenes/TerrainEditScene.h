#pragma once

class TerrainEditScene : public Scene
{
public:
	TerrainEditScene();
	~TerrainEditScene();

	// Scene��(��) ���� ��ӵ�
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