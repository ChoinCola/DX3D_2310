#include "Framework.h"

Pacman::Pacman()
{
	terrain = new Terrain(L"Textures/HeightMaps/Miro.png", 2, true);
	terrain->GetMaterial()->SetDiffuseMap(L"Textures/Landscape/Floor2.png");

	ceiling = new Terrain(L"Textures/Colors/Black.png", 2, true, true);
	ceiling->GetMaterial()->SetDiffuseMap(L"Textures/Landscape/Floor.png");
	ceiling->GetMaterial()->SetNormalMap(L"Textures/Landscape/Floor_normal.png");

	ceiling->SetLocalScale(terrain->GetLocalScale());
	ceiling->SetLocalPosition(Vector3(0, 5, 0));
	ceiling->GetMaterial()->SetSpecularMap(L"Textures/Colors/White.png");


	Settile = new SamplerState();

	player = new ShootingPlayer(Vector3(10, 0, 10));
	player->SetTerrain(terrain);
	SetItem();

	WinBanner = new Quad(Float2(WIN_WIDTH, WIN_HEIGHT));
	WinBanner->GetMaterial()->SetDiffuseMap(L"Textures/UI/WinBanner.png");
	WinBanner->GetMaterial()->SetShader(L"Basic/Texture.hlsl");
	WinBanner->UpdateWorld();

	DefeatBanner = new Quad(Float2(WIN_WIDTH, WIN_HEIGHT));
	DefeatBanner->GetMaterial()->SetDiffuseMap(L"Textures/Colors/Red.png");
	DefeatBanner->GetMaterial()->SetShader(L"Basic/Texture.hlsl");
	DefeatBanner->UpdateWorld();
	//Bullet* dedf = new Bullet(Vector3(1, 0, 1));
	//itemFild.emplace_back(dedf);
}

Pacman::~Pacman()
{
	for (Item* def : itemFild)
		SAFE_DELETE(def);

	SAFE_DELETE(terrain);
	SAFE_DELETE(player);
}

void Pacman::Update()
{
	if (Playnow) {
		for (Item*& def : itemFild)
			def->Update();

		CollisionChack();
		player->Update();
		ceiling->UpdateWorld();
	}
}

void Pacman::PreRender()
{
}

void Pacman::Render()
{
	for (Item*& def : itemFild)
		def->Render();

	//player->Render();
	Render_terrain();
	WinChack();
}

void Pacman::PostRender()
{
	if(Playnow)
		player->PostRender();

}

void Pacman::GUIRender()
{
	ceiling->GUIRender();
	ImGui::Text("Coin \t %d / %d", CountCoin, MaxCoinCount);
	ImGui::Text("Bullet \t %d", CountBullet);
}

void Pacman::Render_terrain()
{
	terrain->Render();
	ceiling->Render();

}

void Pacman::SetItem()
{
	std::random_device rd;  // 랜덤 디바이스
	std::mt19937 generator(rd());  // 머클 트위스터 엔진을 사용
	std::uniform_int_distribution<int> distribution(1, 100);  // 원하는 범위의 분포

	// 난수 생성
	int random_value;
	int item_Count = 20;
	int terX = terrain->GetWidth() / item_Count;
	int terY = terrain->GetHeight() / item_Count;

	// 맵에 아이템 뿌리는 함수
	for (int i = 0; i < item_Count; i++)
	{
		for (int j = 0; j < item_Count; j++)
		{
			random_value = distribution(generator);
			Vector3 onpos = terrain->GetOnGrondPosition(Vector3(i * terX, 0, j * terY)) + Vector3(0, 0.5, 0);
			if (onpos.y < 2)
			{
				Item* insert = new Coin(onpos);;
				if (random_value < 10)
					insert = new Bullet(onpos);
				else {
					insert = new Coin(onpos);
					MaxCoinCount++;
				}

				itemFild.push_back(insert);
			}
		}
	}
}

void Pacman::CollisionChack()
{
	vector<Item*> outlist;
	// 아이템과 플레이어 충돌검사.
	for (Item*& item : itemFild)
	{
		// 충돌시
		if (item->IsCollision(player))
		{
			// 아이템 타입에 따라 갯수 더해주고 렌더중지.
			switch (item->GetItemType())
			{
			case 1:

				item->GetItem(CountBullet);
				break;
			case 2:

				item->GetItem(CountCoin);
				player->GetLightBat() += 10;
				break;
			}
			outlist.emplace_back(item);
		}
	}

	for (Item*& item : outlist)
	{
		itemFild.remove(item);
	}
}

void Pacman::WinChack()
{
	if (CountCoin == MaxCoinCount) {
		WinBanner->SetLocalPosition(Environment::Get()->GetMainCamera()->GetLocalPosition() + Environment::Get()->GetMainCamera()->GetForward());
		WinBanner->SetLocalRotation(Environment::Get()->GetMainCamera()->GetLocalRotation());
		WinBanner->UpdateWorld();
		WinBanner->Render();
		Playnow = false;
	}

	if (player->GetLightBat() < 0)
	{
		DefeatBanner->SetLocalPosition(Environment::Get()->GetMainCamera()->GetLocalPosition() + Environment::Get()->GetMainCamera()->GetForward());
		DefeatBanner->SetLocalRotation(Environment::Get()->GetMainCamera()->GetLocalRotation());
		DefeatBanner->UpdateWorld();
		DefeatBanner->Render();
		Playnow = false;
	}
}
