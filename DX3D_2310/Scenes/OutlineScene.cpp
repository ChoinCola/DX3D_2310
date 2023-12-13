#include "Framework.h"

OutLineScene::OutLineScene()
{
	CreateObjects();

	renderTarget = new RenderTarget();
	depthStencil = new DepthStencil(); // 렌더타겟과 뎁스는 같은 크기를 써야한다.

	Texture* target = Texture::Add(L"Target", renderTarget->GetSRV());
	targetQuad = new Quad(Float2(CENTER_X, CENTER_Y));
	targetQuad->GetMaterial()->SetShader(L"PostEffect/OutLine.hlsl");
	targetQuad->GetMaterial()->SetDiffuseMap(target);
	targetQuad->SetTag("def");

	targetQuad->SetLocalPosition({ CENTER_X * 0.5f, CENTER_Y * 0.5f });

	targetQuad2 = new Quad(Float2(CENTER_X, CENTER_Y));
	targetQuad2->GetMaterial()->SetShader(L"PostEffect/Grayscale.hlsl");
	targetQuad2->GetMaterial()->SetDiffuseMap(target);
	targetQuad2->SetTag("def");
	targetQuad2->SetLocalPosition({ CENTER_X * 0.5f, CENTER_Y * 1.5f });

	valueBuffer = new FloatValueBuffer();
	valueBuffer->GetData()[1] = CENTER_X;
	valueBuffer->GetData()[2] = CENTER_Y;
}

OutLineScene::~OutLineScene()
{
	delete quad;
	delete model;
	delete traveler;
	delete sphere;
}

void OutLineScene::Update()
{
	quad->UpdateWorld();
	model->UpdateWorld();
	traveler->Update();
	sphere->UpdateWorld();
	targetQuad->UpdateWorld();
	targetQuad2->UpdateWorld();
}

void OutLineScene::PreRender()
{
	valueBuffer->SetPS(10);
	renderTarget->Set(depthStencil, {0, 0, 0, 0});

	quad->Render();
	model->Render();
	traveler->Render();
	sphere->Render();
}

void OutLineScene::Render()
{

}

void OutLineScene::PostRender()
{
	targetQuad->Render();
	targetQuad2->Render();
}

void OutLineScene::GUIRender()
{
	ImGui::DragFloat("Scale0", &valueBuffer->GetData()[0], 0.1f);
	ImGui::DragFloat("Scale1", &valueBuffer->GetData()[1], 0.1f);
	ImGui::DragFloat("Scale2", &valueBuffer->GetData()[2], 0.1f);
	targetQuad->GetMaterial()->GUIRender();
}

void OutLineScene::CreateObjects()
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
