#pragma once

class RenderTargetScene : public Scene
{
public:
	RenderTargetScene();
	~RenderTargetScene();

	// Scene��(��) ���� ��ӵ�
	void Update() override;
	void PreRender() override;
	void Render() override;
	void PostRender() override;
	void GUIRender() override;
private:
	void CreateObjects();
private:
	Quad* quad;

	Model* model;

	Traveler* traveler;
	Sphere* sphere;

	RenderTarget* renderTarget;
	DepthStencil* depthStencil;

	Quad* targetQuad;
};