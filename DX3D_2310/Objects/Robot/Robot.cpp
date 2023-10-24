#include "Framework.h"

Robot::Robot(const Float3 pos, float size)
{
	Body1 = new Cube({ 5, 2.5, 2 }, { 0, 7, 0 });

	Head = new Cube({ 1, 1, 1 });
	Vector3 HeadPos = Body1->GetLocalPosition();
	HeadPos.SetY(+Head->GetLocalScale().GetY()*2);
	Head->SetLocalPosition(HeadPos);

	Body2 = new Cube({ 2, 3, 1.5 });
	Vector3 Body2Pos = Body1->GetLocalPosition();
	Body2Pos.SetY(-Body1->GetLocalScale().GetY()*2);
	Body2->SetLocalPosition(Body2Pos);

	Body3 = new Cube({ 4, 1, 2 });
	Vector3 Body3Pos = Body2->GetLocalPosition();
	Body3Pos.SetY(-Body2->GetLocalScale().GetY() - Body3->GetLocalScale().GetY());
	Body3->SetLocalPosition(Body3Pos);

	LLeg = new Cube({ 1, 4, 1 });
	Vector3 LLegPos = Body3->GetLocalPosition();
	LLegPos.SetY(-Body3->GetLocalScale().GetY() * 2);
	LLegPos.SetX(LLegPos.GetX() - Body3->GetLocalScale().GetX() - LLeg->GetLocalScale().GetX()*0.5);
	LLeg->SetLocalPosition(LLegPos);

	Vector3 LLegPivot = {0, LLeg->GetLocalScale().GetY(), 0};
	LLeg->SetPivot(LLegPivot);

	RLeg = new Cube({ 1, 4, 1 }, { +1.5, 1, 1 });
	Vector3 RLegPos = Body3->GetLocalPosition();
	RLegPos.SetY(-Body3->GetLocalScale().GetY() * 2);
	RLegPos.SetX(RLegPos.GetX() + Body3->GetLocalScale().GetX() + RLeg->GetLocalScale().GetX()*0.5);
	RLeg->SetLocalPosition(RLegPos);

	Vector3 RLegPivot = { 0, RLeg->GetLocalScale().GetY(), 0 };
	RLeg->SetPivot(RLegPivot);

	Rshoulder = new Cube({ 1.5, 1.5, 1.5 });
	Vector3 RShoulderPos = Body1->GetLocalPosition();
	RShoulderPos.SetY(+Rshoulder->GetLocalScale().GetY()*0.5);
	RShoulderPos.SetX(RShoulderPos.GetX() + Body1->GetLocalScale().GetX() + Rshoulder->GetLocalScale().GetX()*2);
	Rshoulder->SetLocalPosition(RShoulderPos);

	Vector3 RshoulderPivot = { Body1->GetLocalScale().GetX() / 2, 0, 0 };
	Rshoulder->SetPivot(RshoulderPivot);

	Lshoulder = new Cube({ 1.5, 1.5, 1.5 });
	Vector3 LShoulderPos = Body1->GetLocalPosition();
	LShoulderPos.SetY(+Lshoulder->GetLocalScale().GetY()*0.5);
	LShoulderPos.SetX(LShoulderPos.GetX() - Body1->GetLocalScale().GetX() - Lshoulder->GetLocalScale().GetX()*2);
	Lshoulder->SetLocalPosition(LShoulderPos);

	Vector3 LshoulderPivot = { -Body1->GetLocalScale().GetX() / 2, 0, 0 };
	Lshoulder->SetPivot(LshoulderPivot);

	RArm = new Cube({1, 6, 1});
	Vector3 RArmPos = {};
	RArmPos.SetY(-RArm->GetLocalScale().GetY() * 2 - Rshoulder->GetLocalScale().GetY());
	RArm->SetLocalPosition(RArmPos);

	LArm = new Cube({1, 6, 1});
	Vector3 LArmPos = {};
	LArmPos.SetY(-LArm->GetLocalScale().GetY() * 2 - Lshoulder->GetLocalScale().GetY());
	LArm->SetLocalPosition(LArmPos);

	Head->SetParent(Body1);

	Body2->SetParent(Body1);
	Body3->SetParent(Body2);

	LLeg->SetParent(Body3);
	RLeg->SetParent(Body3);

	Rshoulder->SetParent(Body1);
	Lshoulder->SetParent(Body1);

	RArm->SetParent(Rshoulder);
	LArm->SetParent(Lshoulder);


	part.emplace_back(Body1);
	part.emplace_back(Head);
	part.emplace_back(Body2);
	part.emplace_back(Body3);
	part.emplace_back(LLeg);
	part.emplace_back(RLeg);
	part.emplace_back(Rshoulder);
	part.emplace_back(Lshoulder);
	part.emplace_back(RArm);
	part.emplace_back(LArm);

	for (Cube*& def : part)
	{
		def->GetMetrial()->SetShader(L"Light/NormalMapping.hlsl");
		def->GetMetrial()->SetDiffuseMap(L"Textures/Landscape/Fieldstone_DM.tga");
		def->GetMetrial()->SetSpecularMap(L"Textures/Landscape/Fieldstone_SM.tga");
		def->GetMetrial()->SetNormalMap(L"Textures/Landscape/Fieldstone_NM.tga");
	}
}

Robot::~Robot()
{
	delete Head;
	delete Body1;
	delete Body2;
	delete Body3;

	delete LLeg;
	delete RLeg;

	delete RArm;
	delete Rshoulder;
	delete LArm;
	delete Lshoulder;
}


void Robot::Update()
{
	if (IsDiffuseSet == IsDiffuseSetOld && IsDiffuseSet) {
		for (Cube*& def : part)
			def->GetMetrial()->SetDiffuseMap(L"Textures/Landscape/Fieldstone_DM.tga");
		IsDiffuseSetOld = false;
	}
	else if(IsDiffuseSet == IsDiffuseSetOld && !IsDiffuseSet)
	{
		for (Cube*& def : part)
			def->GetMetrial()->DeleteDiffuseMap();
		IsDiffuseSetOld = true;
	}

	if (IsSpecularMapSet == IsSpecularMapSetOld && IsSpecularMapSet) {
		for (Cube*& def : part)
			def->GetMetrial()->SetSpecularMap(L"Textures/Landscape/Fieldstone_SM.tga");
		IsSpecularMapSetOld = false;
	}
	else if(IsSpecularMapSet == IsSpecularMapSetOld && !IsSpecularMapSet)
	{
		for (Cube*& def : part)
			def->GetMetrial()->DeleteSpecularMap();
		IsSpecularMapSetOld = true;
	}


	if (IsNormalMapSet == IsNormalMapSetOld && IsNormalMapSet) {
		for (Cube*& def : part)
			def->GetMetrial()->SetNormalMap(L"Textures/Landscape/Fieldstone_NM.tga");
		IsNormalMapSetOld = false;
	}
	else if(IsNormalMapSet == IsNormalMapSetOld && !IsNormalMapSet)
	{
		for (Cube*& def : part) {
			def->GetMetrial()->DeleteNormalMap();
		}
		IsNormalMapSetOld = true;
	}


	LLeg->SetRotate({ sin(sinnow), 0, 0});
	RLeg->SetRotate({ -sin(sinnow), 0, 0 });
	Rshoulder->SetRotate({ sin(sinnow), 0, 0 });
	Lshoulder->SetRotate({ -sin(sinnow), 0, 0 });

	for (auto& def : part) {
		def->UpdateWorld();
	}

	if (sinnow <= 180)
		sinnow += DELTA ;
	else
		sinnow = 0;
}


void Robot::Render()
{
	for (auto& def : part) {
		def->Render();
	}
}

void Robot::GUIRender()
{
	ImGui::Checkbox("IsDiffuseSet", &IsDiffuseSet);
	ImGui::Checkbox("IsSpecularMapSet", &IsSpecularMapSet);
	ImGui::Checkbox("IsNormalMapSet", &IsNormalMapSet);
	__super::GUIRender();
}
