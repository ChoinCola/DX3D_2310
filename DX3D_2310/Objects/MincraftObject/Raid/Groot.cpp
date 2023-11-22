#include "Framework.h"

Groot::Groot()
{
	bodyMesh = new ModelAnimator("Traveler");

	ReadClips();
	rightHand = new Transform();

	sword = new Model("Sword");
	sword->SetParent(rightHand);
}

Groot::~Groot()
{
	delete bodyMesh;
	delete sword;
	delete rightHand;
}

void Groot::Update()
{
	Attack();
	UpdateWorld();
	bodyMesh->Update();
	sword->UpdateWorld();
}

void Groot::Render()
{
	Collider::Render();
	bodyMesh->Render();
	sword->Render();
}

void Groot::GUIRender()
{
	bodyMesh->GUIRender();
	sword->GUIRender();
}

void Groot::Attack()
{
	if (KEY->Down(VK_LBUTTON))
	{
		SetState(ATTACK);
	}
}

void Groot::EndAttack()
{
	SetState(IDLE);
}

void Groot::SetState(ActionState state)
{
	if (curstate == state)
		return;
	
	curstate = state;
	bodyMesh->PlayClip(state);
}

void Groot::ReadClips()
{
	bodyMesh->SetParent(this);
	bodyMesh->ReadClip("Idle");
	bodyMesh->ReadClip("Walk");
	bodyMesh->ReadClip("IdleToWalkForward");
	bodyMesh->ReadClip("Right_Walk");
	bodyMesh->ReadClip("IdleTwistR");
	bodyMesh->ReadClip("Left_Walk");
	bodyMesh->ReadClip("IdleTwistL");
	bodyMesh->ReadClip("Back_Walk");
	//ReadClip("Flair");
	bodyMesh->CreateTexture();

	bodyMesh->GetClip(ATTACK)->SeEvent(bind(&Groot::EndAttack, this), 0.8f);


}
