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

void BlockManager::InsertBlocks(Ray mouseray)
{
	list<pair<Contact, Block*>> insertlist;
	bool Mouseinsert = Mouse::Get()->Down(0);

	for (Block* block : blocks)
	{
		Contact scala;
		if (block->IsRayCollision(mouseray, &scala) && Mouseinsert)
		{
			if (insertlist.empty())
				insertlist.emplace_back(scala, block);
			else
			{
				if (insertlist.front().first.distance > scala.distance)
					insertlist.emplace_front(scala, block);
			}
		}
	}

	if (!insertlist.empty()) {
		Block* block = insertlist.front().second;
		Contact scala = insertlist.front().first;

		float length = FLT_MAX;
		Vector3 result;
		FOR(3)
		{
			BoxCollider::ObbDesc desc;
			block->GetObb(desc);

			Vector3 Len = (block->GetLocalPosition() + desc.axis[i] - scala.hitPoint);
			float def = Len.Length();

			if (length >= def) {
				result = desc.axis[i] * desc.halfSize;
				length = def;
			}

			Len = (block->GetLocalPosition() - desc.axis[i] - scala.hitPoint);
			def = def = Len.Length();

			if (length >= def) {
				result = -desc.axis[i] * desc.halfSize;
				length = def;
			}
		}

		result.Normalized();

		int random = MATH->Random(0, 3);
		string name = "Block" + to_string(random);

		Block* newblock = new Block(name);
		newblock->SetLocalPosition(block->GetLocalPosition() + result * block->Getsize().x * 2);

		blocks.push_back(newblock);
		return;
	}
}

void BlockManager::DeleteBlocks(Ray mouseray)
{
	list<pair<float, Block*>> deletelist;
	bool Mouseinsert = Mouse::Get()->Down(1);

	for (Block* block : blocks)
	{
		Contact scala;
		if (block->IsRayCollision(mouseray, &scala) && Mouseinsert)
		{
			if (deletelist.empty())
				deletelist.emplace_back(scala.distance, block);
			else
			{
				if (deletelist.front().first > scala.distance)
					deletelist.emplace_front(scala.distance, block);
			}
		}
	}
	if(!deletelist.empty())
		blocks.remove(deletelist.front().second);
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
