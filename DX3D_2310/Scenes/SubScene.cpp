#include "Framework.h"
#include "SubScene.h"

SubScene::SubScene()
{

	CreateObjects();
	CreateColliderVector();

	DragQuad = new Quad();
	DragQuad->GetMaterial()->SetDiffuseMap(L"Textures/Colors/Green.png");

	renderTarget = new RenderTarget();
	depthStencil = new DepthStencil(); // 렌더타겟과 뎁스는 같은 크기를 써야한다.

	Texture* target = Texture::Add(L"Target", renderTarget->GetSRV());
	targetQuad = new Quad(Float2(WIN_WIDTH, WIN_HEIGHT));
	targetQuad->GetMaterial()->SetShader(L"PostEffect/OutLine.hlsl");
	targetQuad->GetMaterial()->SetDiffuseMap(target);
	targetQuad->SetTag("def");

	targetQuad->SetLocalPosition({ CENTER_X, CENTER_Y });

	valueBuffer = new FloatValueBuffer();
	valueBuffer->GetData()[1] = WIN_WIDTH;
	valueBuffer->GetData()[2] = WIN_HEIGHT;

	rasterizerSate[0] = new RasterizerState();
	rasterizerSate[1] = new RasterizerState();

	rasterizerSate[1]->FillMode(D3D11_FILL_WIREFRAME);

}

SubScene::~SubScene()
{
	delete model;
}

void SubScene::Update()
{
	CollisionChack();
	DragUpdate();
	model->Update();
	targetQuad->UpdateWorld();
}

void SubScene::PreRender()
{
	valueBuffer->SetPS(10);
	renderTarget->Set(depthStencil,{0, 0, 0, 0});

	model->PreRender();

}

void SubScene::Render()
{
	model->Render();
	//for (auto def : collider)
	//	def->Render();
}

void SubScene::PostRender()
{
	targetQuad->Render();
	DragRender();
}

void SubScene::GUIRender()
{
	DragQuad->GUIRender();
	string def = defpos;
	ImGui::Text(def.c_str());
	ImGui::DragInt("Index", &instanceIndex);
	ImGui::DragInt("Clip", &clip);

	if (ImGui::Button("play"))
		model->PlayClip(instanceIndex, clip);

	if (ImGui::Button("SetPreRender"))
		model->SetIsPreRneder(instanceIndex, true);

	ImGui::DragFloat("Scale", &valueBuffer->GetData()[0]);
	ImGui::DragFloat("Scale1", &valueBuffer->GetData()[1]);
	ImGui::DragFloat("Scale2", &valueBuffer->GetData()[2]);
	targetQuad->GetMaterial()->GUIRender();

}

void SubScene::CreateColliderVector()
{

	for (float z = -10.0f; z <= 10.0f; z++)
	{
		for (float x = -10.0f; x <= 10.0f; x++)
		{
			Transform* transform = model->Add();
			transform->SetLocalPosition({ x, 0, z });
			transform->SetLocalRotation({ 0, 0, 0 });
			float scale = MATH->Random(0.5f, 1.0f);
			transform->SetLocalScale(Vector3::One() * scale);
			UnitCollider* def = new UnitCollider(transform);

			def->SetLocalPosition(transform->GetLocalPosition());
			def->SetLocalScale(Vector3(0.5, 0.5, 0.5));
			def->UpdateWorld();

			collider.emplace_back(def);
		}
	}
}

void SubScene::CreateObjects()
{
	model = new ModelAnimatorInstancing("Traveler");

	model->ReadClip("Idle");
	model->ReadClip("Walk");
	model->CreateTexture();
}

void SubScene::CollisionChack()
{
	map<float, UnitCollider*> chacktoupCollider;
	bool isDownKey = KEY->Press(VK_SHIFT);

	bool ClickToTerrain = true;
	for (auto& def : collider)
	{
		Contact output;
		if (def->IsRayCollision(Environment::Get()->GetMainCamera()->ScreenPointToRay(Mouse::Get()->GetPosition()), &output) &&
			Mouse::Get()->Down(0))
		{
			chacktoupCollider[output.distance] = def;

			ClickToTerrain = false;
		}

	}

	if (Mouse::Get()->Press(0))
	{
		for (auto& def : collider)
		{
			Vector3 colscreenpos = Environment::Get()->GetMainCamera()->WorldToScreen(def->GetGlobalPosition());
			if (DragQuad->CollisionChack(colscreenpos))
			{
				def->ClickObject(true);
				ClickToTerrain = false;
			}
		}
		defpos = Environment::Get()->GetMainCamera()->WorldToScreen(collider[0]->GetGlobalPosition());
	}

	if(!ClickToTerrain && !chacktoupCollider.empty())
		(*chacktoupCollider.begin()).second->ClickObject(true);

	if (!isDownKey && ClickToTerrain && Mouse::Get()->Down(0))
	{
		for (auto& def : collider)
		{
			def->ClickObject(false);
		}
	}
}

void SubScene::DragUpdate()
{
	if (Mouse::Get()->Up(0)) {
		DragQuad->SetActive(false);
		DragQuad->SetLocalScale(Vector3(0, 0, 1));
		DragQuadBasePos = {};
	}

	if (Mouse::Get()->Down(0)) {
		DragQuad->SetActive(true);

		Vector3 def = Mouse::Get()->GetPosition();
		def.z = 1;
		DragQuad->SetLocalPosition(def);
		DragQuadBasePos = DragQuad->GetLocalPosition();
	}

	if (Mouse::Get()->Press(0))
	{
		Vector3 moveQuad = {};
		Vector3 scaleQuad = {};

		scaleQuad = Vector3(Mouse::Get()->GetPosition() - DragQuadBasePos);
		moveQuad = Vector3(scaleQuad.x, scaleQuad.y) * 0.5f;

		DragQuad->SetLocalPosition(DragQuadBasePos + moveQuad);
		DragQuad->SetLocalScale(scaleQuad);
	}

	DragQuad->UpdateWorld();
}

void SubScene::DragRender()
{
	rasterizerSate[1]->SetState();
	DragQuad->Render();
	rasterizerSate[0]->SetState();
}
