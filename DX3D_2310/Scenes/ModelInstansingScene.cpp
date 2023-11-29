#include "Framework.h"

ModelInstancingScene::ModelInstancingScene()
{
	modelInstancing = new ModelAnimatorInstancing("Traveler");

	modelInstancing->ReadClip("Idle");
	modelInstancing->ReadClip("Walk");
	modelInstancing->ReadClip("Attack");
	modelInstancing->CreateTexture();
	for (float z = -10.0f; z <= 10.0f; z++)
	{
		for (float x = -10.0f; x <= 10.0f; x++)
		{
			Transform* transform = modelInstancing->Add();
			transform->SetLocalPosition({ x, 0, z });
			float scale = MATH->Random(0.5f, 1.0f);
			transform->SetLocalScale(Vector3::One() * scale);
		}
	}
}

ModelInstancingScene::~ModelInstancingScene()
{
}

void ModelInstancingScene::Update()
{
	modelInstancing->Update();
}

void ModelInstancingScene::PreRender()
{
}

void ModelInstancingScene::Render()
{
	modelInstancing->Render();
}

void ModelInstancingScene::PostRender()
{
}

void ModelInstancingScene::GUIRender()
{
	ImGui::DragInt("Index", &instanceIndex);
	ImGui::DragInt("Clip", &clip);

	if (ImGui::Button("play"))
		modelInstancing->PlayClip(instanceIndex, clip);

	modelInstancing->GUIRender();
}
