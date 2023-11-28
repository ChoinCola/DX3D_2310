#pragma once

class InstancingScene : public Scene
{
private:
	const UINT COUNT = 1000;
public:
	InstancingScene();
	~InstancingScene();

	// Scene을(를) 통해 상속됨
	void Update() override;
	void PreRender() override;
	void Render() override;
	void PostRender() override;
	void GUIRender() override;

private:
	Quad* quad;
	vector<Quad*> quads;

	vector<Matrix> instanceData;
	VertexBuffer* instanceBuffer;
};