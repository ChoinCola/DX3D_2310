#include "Framework.h"
#include "ObejctMaster.h"

ObjectMaster::ObjectMaster()
{
	Trees = new ModelAnimatorInstancing("Tree");
	Grasses = new ModelAnimatorInstancing("Grass");
}

ObjectMaster::~ObjectMaster()
{
	delete Trees;
	delete Grasses;
}

void ObjectMaster::Update()
{
	Trees->Update();
	Grasses->Update();
}

void ObjectMaster::Render()
{
}

// 맵을 렌더하는것
void ObjectMaster::Rendermap(pair<float, float> inputpos, Float2 size)
{
	if (TreePos[inputpos].empty())
	{
		MakeTree(inputpos, size);
		Trees->SetTransforms(&RemakeTransform(&TreePos[inputpos]));
	}
	else
	{
		Trees->SetTransforms(&RemakeTransform(&TreePos[inputpos]));
	}
	Trees->Render();

	if (GrassPos[inputpos].empty())
	{
		MakeGrass(inputpos, size);
		Grasses->SetTransforms(&RemakeTransform(&GrassPos[inputpos]));
	}
	else
	{
		Grasses->SetTransforms(&RemakeTransform(&GrassPos[inputpos]));
	}
	Grasses->Render();

	ColliderRender(inputpos);
}

// 맵의 충돌검사
void ObjectMaster::Collisionmap(pair<float, float> inputpos, Collider* input)
{
	for (auto& coldef : TreePos[inputpos])
		if (coldef->IsCollision(input))
			coldef->SetAction(1);

	for (auto& coldef : GrassPos[inputpos])
		if (coldef->IsCollision(input))
			coldef->SetAction(1);
}

// 지정된 맵의 에니메이션 클립 업데이트
void ObjectMaster::AnimUpdatToemap(pair<float, float> inputpos)
{
	FOR(TreePos[inputpos].size())
		Trees->PlayClip(i, TreePos[inputpos][i]->GetAction());

	FOR(GrassPos[inputpos].size())
		Grasses->PlayClip(i, GrassPos[inputpos][i]->GetAction());
}

void ObjectMaster::GUIRender()
{
}

void ObjectMaster::SetTerrain(vector<Terrain*>* terrain)
{
}

void ObjectMaster::Spawn()
{
}

void ObjectMaster::ColliderRender(pair<float, float> inputpos)
{
	for (auto& def : TreePos[inputpos])
		def->Render();

	for (auto& def : GrassPos[inputpos])
		def->Render();
}

// 나무만들기 함수
void ObjectMaster::MakeTree(pair<float, float> input, Float2 size)
{
	FOR(Treecount)
	{
		Float2 minpos(input.first, input.second);
		Float2 maxpos(input.first + size.x, input.second + size.y);

		Vector3 pos(MATH->Random(minpos.x, maxpos.x),0 ,MATH->Random(minpos.y, maxpos.y));

		pair<float, float> result;

		result.first = input.first;
		result.second = input.second;

		Transform* defTransform = new Transform();
		defTransform->SetLocalPosition(Vector3(pos.x, pos.y, pos.z));

		Tree* deftree = new Tree(defTransform);
		TreePos[input].push_back(deftree);
	}
}

// 잔디만들기 함수
void ObjectMaster::MakeGrass(pair<float, float> input, Float2 size)
{
	FOR(Grasscount)
	{
		Float2 minpos(input.first, input.second);
		Float2 maxpos(input.first + size.x, input.second + size.y);

		Vector3 pos(MATH->Random(minpos.x, maxpos.x), 0, MATH->Random(minpos.y, maxpos.y));

		pair<float, float> result;

		result.first = input.first;
		result.second = input.second;

		Transform* defTransform = new Transform();
		defTransform->SetLocalPosition(Vector3(pos.x, pos.y, pos.z));

		Grass* deftree = new Grass(defTransform);
		GrassPos[input].push_back(deftree);
	}
}
