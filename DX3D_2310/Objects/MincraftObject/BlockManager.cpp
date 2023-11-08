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
	// Vector값을 전부 순환하면서 모든 객체의 데이터를 pos값 위치시키는 함수.
	for (UINT i = 0; i < x; i++) {
		for (UINT j = 0; j < y; j++) {
			for (UINT k = 0; k < z; k++) {
				int random = 100 + MATH->Random(0, BlockDataManager::Get()->GetBlockSize()) + 1;
				BlockData data = BlockDataManager::Get()->GetBlockData(random);

				Block* block = new Block(data.modelname);
				block->SetLocalPosition(Vector3(i, j, k));
				blocks.push_back(block);
			}
		}
	}
}

void BlockManager::CollisionBlockToPlayer(SphereCollider& pos)
{
	// 충돌시. 충돌된 위치에서 pos방향으로 이동한 위치값을 반환함.
	// 아닐경우 자기자신의 위치값을 반환함.
	for (Block*& block : blocks)
	{
		if (block->IsSphereCollision(&pos))
		{
			Vector3 moveposiion = pos.GetGlobalPosition() - pos.GetHitpoint();
			moveposiion.Normalized();

			pos.Translate(moveposiion * (pos.Radius() - pos.Getdistance()));
		}
	}
}

float BlockManager::GetHeight(const Vector3& pos)
{
	// 해당 위치에서 밑으로 뻗어나오는 Ray를 가져온다.
	Ray ray(pos, Vector3::Down());
	Contact contact;

	// 최대 높이는 -5로 정한다. 최저높이를 제한함.
	float maxHeight = -5.0f;

	// 블록 전체를 순환하면서. 가장 가까운 블록을 갱신처리한다.
	for (Block*& block : blocks)
	{
		if (block->IsRayCollision(ray, &contact));
		{
			if (contact.hitPoint.y > maxHeight)
				maxHeight = contact.hitPoint.y;
		}
	}
	//최대 높이는 객체에서 충돌범위까지 가장 적은 거리를 가진 블록의 높이값을 반환하게 한다.
	return maxHeight;
}

Block* BlockManager::DeleteBlock(const Ray& ray)
{
	float minDistance = FLT_MAX;
	Contact contact;

	list<Block*>::iterator iter = blocks.begin();
	list<Block*>::iterator collisionBlock = blocks.end();

	for (; iter != blocks.end(); iter++) {
		if ((*iter)->IsRayCollision(ray, &contact))
		{
			if (contact.distance < minDistance)
			{
				minDistance = contact.distance;
				collisionBlock = iter;
			}
		}
	}

	if (collisionBlock == blocks.end())
		return nullptr;

	Block* block = *collisionBlock;
	blocks.erase(collisionBlock);

	return block;
}

void BlockManager::AddBlock(Block* block)
{
	// 카메라의 중앙에서 뻗어나오는 Ray를 가져온다.
	//Ray ray = CAM->ScreenPointToRay(Vector3(CENTER_X, CENTER_Y, 0));
	Ray ray = CAM->ScreenPointToRay(Mouse::Get()->GetPosition());

	// 가장 최소거리에 float값의 최대값을 넣어서. 다음값이 무조건 최소가 될 수 있도록 한다.
	float minDistance = FLT_MAX;

	// 접촉부
	Contact contact;
	Vector3 hitPoint;

	// 리스트의 iterator를 사용하는 방식. 좀 구식방법
	list<Block*>::iterator iter = blocks.begin();
	list<Block*>::iterator collisionBlock = blocks.end();

	// 이터레이터를 순환하며. 전체 Ray검사를 실시하고. 가장 가까운 block을 찾아 갱신한다.
	for (; iter != blocks.end(); iter++)
	{
		if ((*iter)->IsRayCollision(ray, &contact))
		{
			if (contact.distance < minDistance)
			{
				minDistance = contact.distance;
				collisionBlock = iter;
				hitPoint = contact.hitPoint;
			}
		}
	}

	// 만약 충돌블록이 마지막이라면 충돌 안했다는 의미임으로. 반환한다.(보통 iter의 마지막값은 마지막요소가 아닌
	// 리스트의 끝을 의미함. 
	if (collisionBlock == blocks.end())
		return;

	// 충돌블록중 가장 가까운 블록에 collisionBlock을 대입.
	Block* hitBlock = *collisionBlock;

	// 충돌거리는 hitBlock의 hitpoint를 대입한다.
	Vector3 direction = hitBlock->GetDirection(hitPoint);
	// 충돌된 위치의 hitpoint는. hitBlock의 GlobalPosition의 direction만큼 위치이동한곳을 지정한다.
	// 블록의 사이즈는 Normalize되었음으로 그냥 위치이동시켜주면 그곳이 생성부가 된다.
	Vector3 pos = hitBlock->GetGlobalPosition() + direction;

	block->SetLocalPosition(pos);
	block->Update();
	blocks.push_back(block);
}
