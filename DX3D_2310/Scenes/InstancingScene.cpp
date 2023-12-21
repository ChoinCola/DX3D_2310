#include "Framework.h"

InstancingScene::InstancingScene()
{
	//quads.resize(COUNT);
	//for (Quad*& quad : quads)
	//{
	//	quad = new Quad();
	//	quad->GetMaterial()->SetDiffuseMap(L"Textures/Landscape/Box.png");

	//	Vector3 pos(MATH->Random(-10, 10), MATH->Random(-10, 10));
	//	quad->SetLocalPosition(pos);
	//	quad->UpdateWorld();
	//}

	quad = new Quad();
	quad->GetMaterial()->SetDiffuseMap(L"Textures/Landscape/Box.png");
	quad->GetMaterial()->SetShader(L"Instancing/TextureInstancing.hlsl");

	instanceData.resize(COUNT);

	for (Matrix& transform : instanceData)
	{
		Vector3 pos(MATH->Random(-100, 100), MATH->Random(-100, 100));
		pos += Vector3(WIN_WIDTH * 0.5, WIN_HEIGHT * 0.5);
		transform = XMMatrixScaling(100, 100, 1);
		transform *= XMMatrixTranslation(pos.x, pos.y, pos.z);
		
		transform = XMMatrixTranspose(transform); // 전치행렬화
	}

	instanceBuffer = new VertexBuffer(instanceData.data(), sizeof(Matrix), COUNT);
}

InstancingScene::~InstancingScene()
{
	delete quad;
	delete instanceBuffer;
}

void InstancingScene::Update()
{
}

void InstancingScene::PreRender()
{
}

void InstancingScene::Render()
{
	//for (Quad* quad : quads)
	//	quad->Render();

	//instanceBuffer->Set(1);
	//quad->RenderInstanced(COUNT);
}

void InstancingScene::PostRender()
{
	instanceBuffer->Set(1);
	quad->RenderInstanced(COUNT);
}

void InstancingScene::GUIRender()
{
}
