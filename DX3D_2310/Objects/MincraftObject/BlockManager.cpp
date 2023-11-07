#include "Framework.h"

BlockManager::BlockManager()
{
}

BlockManager::~BlockManager()
{
	for (Block* block : blocks)
		SAFE_DELETE(block);
}

void BlockManager::Update()
{
	for (Block* block : blocks)
		block->Update();
}

void BlockManager::Redner()
{
	for (Block* block : blocks)
		block->Render();
}

void BlockManager::GUIRender()
{
}

void BlockManager::CreateBlocks(UINT x, UINT y, UINT z)
{
	for (UINT i = 0; i < x; i++) {
		for (UINT j = 0; j < y; j++) {
			for (UINT k = 0; k < z; k++) {
				int random = MATH->Random(0, 3);
				string name = "Block" + to_string(random);

				Block* block = new Block(name);
				block->SetLocalPosition(Vector3(i, j, k));
				blocks.push_back(block);
			}
		}
	}
}
