#pragma once

class BillboardScene : public Scene
{
private:
	const UINT COUNT = 1000;

public:
	BillboardScene();
	~BillboardScene();

	// Scene��(��) ���� ��ӵ�
	void Update() override;
	void PreRender() override;
	void Render() override;
	void PostRender() override;
	void GUIRender() override;

private:
	Terrain* terrain;
	//vector<Quad*> trees;

	DepthStencilState* depthState[2];

	Material* material;
	VertexBuffer* vertexBuffer;
	GeometryShader* geometryShader;

	vector<VertexUV> vertices;
	vector<VertexUV> drawVertices;
};