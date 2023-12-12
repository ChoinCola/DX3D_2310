#include "Framework.h"
#include "RenderTargetScene.h"

RenderTargetScene::RenderTargetScene()
{
	CreateObjects();

	renderTarget = new RenderTarget();
	depthStencil = new DepthStencil(); // ����Ÿ�ٰ� ������ ���� ũ�⸦ ����Ѵ�.

	Texture* target = Texture::Add(L"Target", renderTarget->GetSRV());
	targetQuad = new Quad(Float2(WIN_WIDTH, WIN_HEIGHT));
	targetQuad->GetMaterial()->SetShader(L"PostEffect/Sepia.hlsl");
	targetQuad->GetMaterial()->SetDiffuseMap(target);
	targetQuad->SetTag("def");

	targetQuad->SetLocalPosition({ CENTER_X, CENTER_Y });
}

RenderTargetScene::~RenderTargetScene()
{
	delete quad;
	delete model;
	delete traveler;
	delete sphere;
}

void RenderTargetScene::Update()
{
	quad->UpdateWorld();
	model->UpdateWorld();
	traveler->Update();
	sphere->UpdateWorld();
	targetQuad->UpdateWorld();
}

void RenderTargetScene::PreRender()
{
	renderTarget->Set(depthStencil);

	quad->Render();
	model->Render();
	traveler->Render();
	sphere->Render();
}

void RenderTargetScene::Render()
{

}

void RenderTargetScene::PostRender()
{
	targetQuad->Render();
}

void RenderTargetScene::GUIRender()
{
	//quad->		GUIRender();
	//model->		GUIRender();
	//traveler->	GUIRender();
	//sphere->	GUIRender();
	//targetQuad->GUIRender();

	targetQuad->GetMaterial()->GUIRender();
}

void RenderTargetScene::CreateObjects()
{
	quad = new Quad();
	quad->Load();

	model = new Model("StanfordBunny");
	model->Load();

	traveler = new Traveler();
	traveler->Load();

	sphere = new Sphere();
	//sphere->GetMaterial()->Load("TextData/Materials/1.mat");
	sphere->Load();
}
