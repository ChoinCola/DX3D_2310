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
	// Vector���� ���� ��ȯ�ϸ鼭 ��� ��ü�� �����͸� pos�� ��ġ��Ű�� �Լ�.
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
	// �浹��. �浹�� ��ġ���� pos�������� �̵��� ��ġ���� ��ȯ��.
	// �ƴҰ�� �ڱ��ڽ��� ��ġ���� ��ȯ��.
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
	// �ش� ��ġ���� ������ ������� Ray�� �����´�.
	Ray ray(pos, Vector3::Down());
	Contact contact;

	// �ִ� ���̴� -5�� ���Ѵ�. �������̸� ������.
	float maxHeight = -5.0f;

	// ��� ��ü�� ��ȯ�ϸ鼭. ���� ����� ����� ����ó���Ѵ�.
	for (Block*& block : blocks)
	{
		if (block->IsRayCollision(ray, &contact));
		{
			if (contact.hitPoint.y > maxHeight)
				maxHeight = contact.hitPoint.y;
		}
	}
	//�ִ� ���̴� ��ü���� �浹�������� ���� ���� �Ÿ��� ���� ����� ���̰��� ��ȯ�ϰ� �Ѵ�.
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

bool BlockManager::AddBlock(Block* block)
{
	// ī�޶��� �߾ӿ��� ������� Ray�� �����´�.
	//Ray ray = CAM->ScreenPointToRay(Vector3(CENTER_X, CENTER_Y, 0));
	Ray ray = CAM->ScreenPointToRay(Mouse::Get()->GetPosition());

	// ���� �ּҰŸ��� float���� �ִ밪�� �־. �������� ������ �ּҰ� �� �� �ֵ��� �Ѵ�.
	float minDistance = FLT_MAX;

	// ���˺�
	Contact contact;
	Vector3 hitPoint;

	// ����Ʈ�� iterator�� ����ϴ� ���. �� ���Ĺ��
	list<Block*>::iterator iter = blocks.begin();
	list<Block*>::iterator collisionBlock = blocks.end();

	// ���ͷ����͸� ��ȯ�ϸ�. ��ü Ray�˻縦 �ǽ��ϰ�. ���� ����� block�� ã�� �����Ѵ�.
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

	// ���� �浹����� �������̶�� �浹 ���ߴٴ� �ǹ�������. ��ȯ�Ѵ�.(���� iter�� ���������� ��������Ұ� �ƴ�
	// ����Ʈ�� ���� �ǹ���. 
	if (collisionBlock == blocks.end())
		return false;

	// �浹����� ���� ����� ��Ͽ� collisionBlock�� ����.
	Block* hitBlock = *collisionBlock;

	// �浹�Ÿ��� hitBlock�� hitpoint�� �����Ѵ�.
	Vector3 direction = hitBlock->GetDirection(hitPoint);
	// �浹�� ��ġ�� hitpoint��. hitBlock�� GlobalPosition�� direction��ŭ ��ġ�̵��Ѱ��� �����Ѵ�.
	// ����� ������� Normalize�Ǿ������� �׳� ��ġ�̵������ָ� �װ��� �����ΰ� �ȴ�.
	Vector3 pos = hitBlock->GetGlobalPosition() + direction;

	block->SetLocalPosition(pos);
	block->Update();
	blocks.push_back(block);
	return true;
}
