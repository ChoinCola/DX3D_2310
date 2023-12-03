#include "Framework.h"
#include "ObejctMaster.h"

ObjectMaster::ObjectMaster()
{
	//Trees = new ModelAnimatorInstancing("Tree");
	//Trees->ReadClip("Idle");
	//Trees->ReadClip("Shack");
	//Trees->CreateTexture();

	Grasses = new ModelAnimatorInstancing("Grass");
	Grasses->SetTag("Grass_map");
	Grasses->Load();
	Grasses->SetLocalRotation(Vector3(1.6, 0, 0));
	Grasses->ReadClip("Idle");
	Grasses->ReadClip("Shack");
	Grasses->CreateTexture();
	Grasses->UpdateWorld();
}

ObjectMaster::~ObjectMaster()
{
	delete Trees;
	Grasses->Save();
	delete Grasses;
}

void ObjectMaster::Update()
{
	//Trees->Update();
	//Grasses->Update();
}

void ObjectMaster::Render()
{
}

// 맵을 렌더하는것
void ObjectMaster::Rendermap(pair<float, float> inputpos, Float2 size)
{
	//if (TreePos[inputpos].empty())
	//{
	//	MakeTree(inputpos, size);
	//	Trees->SetTransforms(&RemakeTransform(&TreePos[inputpos]));
	//}
	//else
	//{
	//	Trees->SetTransforms(&RemakeTransform(&TreePos[inputpos]));
	//}
	//Trees->Render();
	vector<Transform*> definput;
	if (GrassPos[inputpos].empty())
		MakeGrass(inputpos, size);

	//for (auto& def : GrassPos[inputpos])
	//{
	//	if(def->GetObejctnum() == -1)
	//		inputTransform(def, Grasses);
	//}
	RemakeTransform(GrassPos[inputpos], definput);
	Grasses->SetTransforms(definput);
	Grasses->Update();
	Grasses->Render();

	ColliderRender(inputpos);
}

// 맵의 충돌검사
void ObjectMaster::Collisionmap(pair<float, float> inputpos, Collider* input)
{
	//for (auto& coldef : TreePos[inputpos])
	//	if (coldef->IsCollision(input))
	//		coldef->SetAction(1);

	FOR(GrassPos[inputpos].size())
		if (GrassPos[inputpos][i]->IsCollision(input)) {

			GrassPos[inputpos][i]->SetAction(0);
			GrassPos[inputpos][i]->SetColor(Float4(1, 0, 0, 1));
		}
		else
		{

			GrassPos[inputpos][i]->SetColor(Float4(0, 1, 0, 1));
			GrassPos[inputpos][i]->SetAction(1);
			Grasses->PlayClip(i, GrassPos[inputpos][i]->GetAction(), 1.0f, 0.0f);
		}
}

// 지정된 맵의 에니메이션 클립 업데이트
void ObjectMaster::AnimUpdatToemap(pair<float, float> inputpos)
{
	//FOR(TreePos[inputpos].size())
	//	Trees->PlayClip(i, TreePos[inputpos][i]->GetAction());

	FOR(GrassPos[inputpos].size())
		Grasses->PlayClip(i, GrassPos[inputpos][i]->GetAction(),0.1f,0.0f);
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

void ObjectMaster::GUIRendermap(pair<float, float> inputpos)
{

	ImGui::DragInt("Index", &instanceIndex);
	ImGui::DragInt("Clip", &clip,0.01,0,2);

	if (ImGui::Button("play"))
		Grasses->PlayClip(instanceIndex, clip);

	for (auto& def : TreePos[inputpos])
		def->GUIRender();

	for (auto& def : GrassPos[inputpos]) {
		def->GUIRender();


	}

	Grasses->GUIRender();
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

		Object* deftree = new Object(defTransform);
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

		Transform* defTransform = new Transform();
		defTransform->SetLocalPosition(Vector3(pos.x, pos.y, pos.z));
		defTransform->SetLocalRotation(Vector3(1.6, 0, 0));
		Object* deftree = new Object(defTransform);
		deftree->SetActive(true);
		GrassPos[input].push_back(deftree);
		delete defTransform;
	}
}

void ObjectMaster::inputTransform(Object* input, ModelAnimatorInstancing* output)
{
	Transform* def = output->Add();

	def->SetLocalPosition(input->GetLocalPosition());
	def->SetLocalRotation(input->GetLocalRotation());
	def->SetLocalScale(input->GetLocalScale());

	input->SetObejctnum(output->GetLastObjectNum());
}

void ObjectMaster::OutputmapToObject(pair<float, float> input)
{
	for (auto& def : GrassPos[input])
	{
		Grasses->TransformUnActive((def->GetObejctnum()));
		def->SetObejctnum(-1);
	}
}

void ObjectMaster::RemakeTransform(vector<Object*> input, vector<Transform*>& output)
{
	output.reserve(input.size());  // 최적화: result 벡터의 크기를 미리 예약

	for (auto& element : input) {
		// Transform 클래스에 대한 변환 로직을 구현해야 함
		Transform* transformedElement = element;
		output.push_back(transformedElement);
	}
}
