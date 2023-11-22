#include "Framework.h"
#include "AnimationScene.h"

AnimationScene::AnimationScene()
{
	model = new ModelAnimator("Traveler");
	model->ReadClip("Walk");
	model->ReadClip("Right_Walk");
	model->ReadClip("Flair");
	model->CreateTexture();

	model->GetClip(1)->SetEvent(bind(&AnimationScene::End, this), 0.5f);



	//model->SetLocalScale(Vector3(0.01, 0.01, 0.01));

	//weapon = new Model("weapon0");

	//rightHand = new Transform();
	//rightHand->SetWorld(model->GetNodeTransform("mixamorig:RightHand"));
	////rightHand->SetParent(model);

	//weapon->SetParent(rightHand);

}

AnimationScene::~AnimationScene()
{
	SAFE_DELETE(model);
	//SAFE_DELETE(weapon);
}

void AnimationScene::Update()
{
	if (KEY->Down('1'))
	{
		model->PlayClip(0);
	}

	model->Update();
	//rightHand->SetWorld(model->GetNodeTransform("mixamorig:RightHand"));
	////rightHand->UpdateWorld();
	//weapon->UpdateWorld();
}

void AnimationScene::PreRender()
{
}

void AnimationScene::Render()
{
	model->Render();
	//weapon->Render();
}

void AnimationScene::PostRender()
{
}

void AnimationScene::GUIRender()
{
	model->GUIRender();
	//weapon->GUIRender();
}

void AnimationScene::End()
{
	model->PlayClip(1);
}
