#include "Framework.h"
#include "RenderTargetScene.h"

RenderTargetScene::RenderTargetScene()
{
	CreateObjects();

	renderTarget = new RenderTarget();
	depthStencil = new DepthStencil(); // ����Ÿ�ٰ� ������ ���� ũ�⸦ ����Ѵ�.

	Texture* target = Texture::Add(L"Target", renderTarget->GetSRV());
	targetQuad = new Quad(Float2(CENTER_X, CENTER_Y));
	targetQuad->GetMaterial()->SetShader(L"PostEffect/Mosaic.hlsl");
	targetQuad->GetMaterial()->SetDiffuseMap(target);
	targetQuad->SetTag("def");

	targetQuad->SetLocalPosition({ CENTER_X * 0.5f, CENTER_Y * 0.5f});

	targetQuad2 = new Quad(Float2(CENTER_X, CENTER_Y));
	targetQuad2->GetMaterial()->SetShader(L"PostEffect/Grayscale.hlsl");
	targetQuad2->GetMaterial()->SetDiffuseMap(target);
	targetQuad2->SetTag("def");
	targetQuad2->SetLocalPosition({ CENTER_X*0.5f, CENTER_Y*1.5f});

	valueBuffer = new FloatValueBuffer();
	valueBuffer->GetData()[1] = CENTER_X;
	valueBuffer->GetData()[2] = CENTER_Y;
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
	targetQuad2->UpdateWorld();
}

void RenderTargetScene::PreRender()
{
	valueBuffer->SetPS(10);
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
	targetQuad2->Render();
}

void RenderTargetScene::GUIRender()
{
	//quad->		GUIRender();
	//model->		GUIRender();
	//traveler->	GUIRender();
	//sphere->	GUIRender();
	//targetQuad->GUIRender();
	ImGui::DragFloat("Scale", &valueBuffer->GetData()[0]);
	ImGui::DragFloat("Scale1", &valueBuffer->GetData()[1]);
	ImGui::DragFloat("Scale2", &valueBuffer->GetData()[2]);
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
