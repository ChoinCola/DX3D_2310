#include "Framework.h"

MineCraftUI::MineCraftUI()
{
	cursor = new Quad(L"Textures/UI/Cursor.png");
	cursor->SetLocalPosition({ CENTER_X, CENTER_Y, 0.0f });
	cursor->UpdateWorld();
}

MineCraftUI::~MineCraftUI()
{
	delete cursor;
	
	for (Block* block : blocks)
		delete block;
}

void MineCraftUI::Update()
{
	cursor->UpdateWorld();
}

void MineCraftUI::PostRender()
{
	cursor->Render();
}

void MineCraftUI::Mining()
{
	//Ray ray = CAM->ScreenPointToRay(Vector3(CENTER_X, CENTER_Y, 0));
	Ray ray = CAM->ScreenPointToRay(Mouse::Get()->GetPosition());

	Block* block = BlockManager::Get()->DeleteBlock(ray);

	if (block == nullptr)
		return;

	blocks.push_back(block);
}

void MineCraftUI::Build()
{
	if (blocks.size() == 0) return;

	Block* block = blocks.back();
	blocks.pop_back();

	BlockManager::Get()->AddBlock(block);

}
