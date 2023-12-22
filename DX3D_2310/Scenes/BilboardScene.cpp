#include "Framework.h"

BillboardScene::BillboardScene()
{
	terrain = new Terrain();

    trees.reserve(COUNT);
    FOR(COUNT)
    {
        Float2 size;
        size.x = MATH->Random(3.0f, 10.0f);
        size.y = MATH->Random(3.0f, 10.0f);

        Quad* tree = new Quad(size);
        tree->GetMaterial()->SetDiffuseMap(L"Textures/Landscape/Tree.png");
        Vector3 pos;
        pos.x = MATH->Random(0.0f, terrain->GetSize().x);
        pos.z = MATH->Random(0.0f, terrain->GetSize().y);
        pos.y = terrain->GetOnGrondPosition(pos).y + size.y * 0.5f;
        tree->SetLocalPosition(pos);

        trees.push_back(tree);
    }

    depthState[0] = new DepthStencilState();
    depthState[1] = new DepthStencilState();
    depthState[1]->DepthWriteMask(D3D11_DEPTH_WRITE_MASK_ZERO); // 깊이연산을 하지 마 라고 하는것.
}

BillboardScene::~BillboardScene()
{
    delete terrain;
    for (Quad* tree : trees)
        delete tree;
}

void BillboardScene::Update()
{
    for (Quad* tree : trees)
    {
        //tree->SetLocalRotation(CAM->GetLocalRotation());
        //tree->SetLocalRotation({ 0,CAM->GetLocalRotation().y,0 });
        Vector3 dir = tree->GetLocalPosition() - CAM->GetLocalPosition();
        tree->SetLocalRotation({ 0, atan2(dir.x, dir.z), 0 });
        tree->UpdateWorld();
    }
}

void BillboardScene::PreRender()
{
}

void BillboardScene::Render()
{
    terrain->Render();
    depthState[1]->SetState();
    for (Quad* tree : trees)
        tree->Render();
    depthState[0]->SetState();
}

void BillboardScene::PostRender()
{
}

void BillboardScene::GUIRender()
{
}
