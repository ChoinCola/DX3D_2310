#pragma once

class BillboardScene : public Scene
{
private:
	const UINT COUNT = 1000;

public:
	BillboardScene();
	~BillboardScene();

	// Scene을(를) 통해 상속됨
	void Update() override;
	void PreRender() override;
	void Render() override;
	void PostRender() override;
	void GUIRender() override;

private:
	Terrain* terrain;
	vector<Quad*> trees;

	DepthStencilState* depthState[2];
};