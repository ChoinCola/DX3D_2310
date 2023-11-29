#pragma once

class Trees : public SphereCollider
{
	const UINT COUNT = 100;

public:
	Trees();
	~Trees();
	 
	void Update();
	void Render();
	void PostRender();
	void RenderTerraintoTree(pair<float, float> nowpos, Float2 size);

private:
	void ChackRaytoCAM();
	void SetColor();
	void MoveAnimation();
	void MakeTree(pair<float, float> nowpos, Float2 size);
	void RenderTree(pair<float, float> nowpos);

private:
	map<pair<float, float>, vector<Matrix>> treesmap;
	map<pair<float, float>, VertexBuffer*> instanceBuffermap;
	//VertexBuffer* instanceBuffer;

	Model* model = nullptr;
};