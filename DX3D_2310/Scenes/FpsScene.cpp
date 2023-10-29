#include "Framework.h"
#include "FpsScene.h"

FpsScene::FpsScene()
{
	CamCollider = new SphereCollider();
	terrain = new Terrain();
	terrain->GetMetrial()->SetShader(L"Light/NormalMapping.hlsl");
	terrain->GetMetrial()->SetDiffuseMap	(L"Textures/Landscape/Fieldstone_DM.tga");
	terrain->GetMetrial()->SetSpecularMap(L"Textures/Landscape/Fieldstone_SM.tga");
	terrain->GetMetrial()->SetNormalMap	(L"Textures/Landscape/Fieldstone_NM.tga");

	listMaxSize = listSize = 0;
	listMaxSize = 10;
	Ball_speed = 30;
}

FpsScene::~FpsScene()
{

}

void FpsScene::Update()
{
	if (Start == true) {
		// 구체가 최대치가 아닐경우 만듬.
		MakeBallList();
		// 구체가 카메라로 계속 이동하게 만듬.
		MoveBallList();
		// 구체의 충돌확인.
		ChackBall();
		// 카메라용 Collider
		CamCollider->SetLocalPosition(terrain->GetOnGrondPosition(CAM->GetLocalPosition()));
		CamCollider->UpdateWorld();

		for (auto& def : Balllist)
			def->Update();

	}

	if (HP == 0 || Wincount == 0) {
		End = true;
		Start = false;
	}
}

void FpsScene::PreRender()
{
}

void FpsScene::Render()
{
	for (auto& def : Balllist) {
		def->Render();

	}
	terrain->Render();
}

void FpsScene::PostRender()
{
}

void FpsScene::GUIRender()
{
	ImGui::Text("Hitcount : %d", Hitcount);
	ImGui::Text("HP : %d", HP);
	ImGui::Text("WinCount : %d", Wincount);


	ImGui::Text("Ball : %d", Balllist.size());
	ImGui::SliderFloat("Ball_Speed", &Ball_speed, 0, 100);

	ImGui::Checkbox("Start", &Start);

	if (Start == true && (HP <= 0 || Wincount <= 0)) {
		HP = 10;
		End = false;
		Wincount = 20;
	}

	if (End == true)
	{
		if (HP <= 0)
		{
			ImGui::Text("False");
		}
		else
			ImGui::Text("Success");
	}
}

void FpsScene::ChackBall()
{
	Ray ray = CAM->ScreenPointToRay(Mouse::Get()->GetPosition());
	Contact contact;

	for (auto& def : Balllist)
	{
		if (def->IsRayCollision(ray, &contact) && KEY->Down(VK_LBUTTON))
		{
			PosResetBall(def);
			Wincount--;
		}
		else
		if (CamCollider->IsSphereCollision(def))
		{
			PosResetBall(def);
			Hitcount++;
			HP--;
		}
	}
}

void FpsScene::MakeBallList()
{
	if (listMaxSize > Balllist.size())
	{
		uniform_int_distribution<int> Posrandom(0, 360);
		std::mt19937 gen(rd());
		float randomnum = Posrandom(gen);
		Ball* newball = new Ball(3);
		newball->GetSphere()->GetMetrial()->SetDiffuseMap(L"Textures/Landscape/Box.png");
		newball->SetLocalPosition(
			Vector3(sin(randomnum) * terrain->GetWidth()*0.5, 0, cos(randomnum) * terrain->GetHeight()*0.5) + 
			Vector3(terrain->GetWidth() * 0.5, 0, terrain->GetHeight() * 0.5));

		Balllist.push_back(newball);
	}
}

void FpsScene::MoveBallList()
{
	Vector3 Campos = CamCollider->GetLocalPosition();

	for (auto& def : Balllist)
	{
		Vector3 ballpos = def->GetLocalPosition();
		Vector3 MovePos = Campos - ballpos;

		MovePos.Normalized();
		def->SetLocalPosition(terrain->GetOnGrondPosition(ballpos + MovePos * Ball_speed * DELTA));
	}
}

void FpsScene::PosResetBall(Ball*& input)
{
	uniform_int_distribution<int> Posrandom(0, 360);
	std::mt19937 gen(rd());
	float randomnum = Posrandom(gen);

	input->SetLocalPosition(
		Vector3(sin(randomnum) * terrain->GetWidth() * 0.5, 0, cos(randomnum) * terrain->GetHeight() * 0.5) +
		Vector3(terrain->GetWidth() * 0.5, 0, terrain->GetHeight() * 0.5));
}
