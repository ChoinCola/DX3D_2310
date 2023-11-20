#include "Framework.h"
#include "AnimationScene.h"

AnimationScene::AnimationScene()
{
	model = new ModelAnimator("Traveler");
	model->ReadClip("Walk");
	model->ReadClip("Right_Walk");
	model->ReadClip("Flair");
	model->CreateTexture();

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
