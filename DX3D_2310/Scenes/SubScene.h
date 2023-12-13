#pragma once

class SubScene : public Scene
{
public:
	SubScene();
	~SubScene();

	// Scene을(를) 통해 상속됨
	void Update() override;
	void PreRender() override;
	void Render() override;
	void PostRender() override;
	void GUIRender() override;

private:
	void CreateColliderVector();
	void CreateObjects();
	void CollisionChack();
	
private:
	void DragUpdate();
	void DragRender();

private:
	ModelAnimatorInstancing* model;
	vector<UnitCollider*> collider;

	RenderTarget* renderTarget;
	DepthStencil* depthStencil;

	Quad* DragQuad;
	Vector3 DragQuadBasePos = {};

	Vector3 defpos = {};
	Quad* targetQuad;

	FloatValueBuffer* valueBuffer;

	RasterizerState* rasterizerSate[2];

	int instanceIndex = 0;
	int clip;
};