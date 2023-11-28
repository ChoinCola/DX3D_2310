#include "Framework.h"
#include "Trees.h"

Trees::Trees()
{
	model = new Model("block0");
	model->SetLocalScale(Vector3(0.05, 0.05, 0.05));
	model->UpdateWorld();
	model->SetShader(L"Model/ModelInstancing.hlsl");
}

Trees::~Trees()
{
}

void Trees::Update()
{
}

void Trees::Render()
{

}

void Trees::PostRender()
{
}

void Trees::RenderTerraintoTree(pair<float, float> nowpos, Float2 size)
{
	if (treesmap.count(nowpos) == 0)
		MakeTree(nowpos, size);

	RenderTree(nowpos);
}

void Trees::ChackRaytoCAM()
{
}

void Trees::SetColor()
{
}

void Trees::MoveAnimation()
{
}

void Trees::MakeTree(pair<float, float> nowpos, Float2 size)
{
	FOR(COUNT)
	{
		Float2 minpos(nowpos.first, nowpos.second);
		Float2 maxpos(nowpos.first + size.x, nowpos.second + size.y);

		Vector3 pos(MATH->Random(minpos.x, maxpos.x),0 ,MATH->Random(minpos.y, maxpos.y));

		pair<float, float> result;

		result.first = nowpos.first;
		result.second = nowpos.second;

		Matrix insret = XMMatrixTranslation(pos.x, pos.y, pos.z);
		insret = XMMatrixTranspose(insret);
		treesmap[result].push_back(insret); // 전치행렬화
	}
}

void Trees::RenderTree(pair<float, float> nowpos)
{
	instanceBuffer = new VertexBuffer(treesmap[nowpos].data(), sizeof(Matrix), COUNT);
	instanceBuffer->Set(1);
	model->RenderInstanced(COUNT);
	delete instanceBuffer;
}
