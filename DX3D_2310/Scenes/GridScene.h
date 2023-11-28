#pragma once

class GridScene : public Scene
{
private:
	const int MAX_SIZE = 100;

public:
	GridScene();
	~GridScene();

	// Scene을(를) 통해 상속됨
	void Update() override;
	void PreRender() override;
	void Render() override;
	void PostRender() override;
	void GUIRender() override;

private:
	void MakeMesh();
private:
	Material* material;
	Mesh<VertexColor>* mesh;
	MatrixBuffer* worldBuffer;

	UINT width, height;

	SkyBox* skyBox;
};