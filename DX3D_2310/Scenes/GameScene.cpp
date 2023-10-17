#include "Framework.h"

GameScene::GameScene()
{
	MovePoint = new GameBox();
	MovePoint->SetImage(L"Textures/Localgame/MovePoint.jpg");
	MovePoint->SetLocalPosition({ MAX_SIZE-1, 1, 0 });
	Swap1 = Swap2 = MovePoint->GetLocalPosition();

	for (float i = 0; i < pow(MAX_SIZE, 2) - 1; i++) {
		Vector3 pos = { (float)((int)i % (int)MAX_SIZE), MAX_SIZE - (int)(i / MAX_SIZE), 0 };
		GameBox* inputBox = new GameBox(MAX_SIZE, i, Float3{ 1,1,1 }, pos);
		inputBox->SetImage(L"Textures/Localgame/BaseGameImage.jpg");
		GameBord[i] = inputBox;
	}
	GameBord[pow(MAX_SIZE, 2)-1] = MovePoint;

	Sw2 = nowPos = pow(MAX_SIZE, 2) - 1;

	for (float i = 0; i < pow(MAX_SIZE, 2); i++)
		GameBord[i]->Update();
	BoxMIX();
	Moving = false;
}

GameScene::~GameScene()
{
	for (float i = 0; i < pow(MAX_SIZE, 2); i++)
		delete GameBord[i];
}

void GameScene::Update()
{
	Move();
	for (float i = 0; i < pow(MAX_SIZE, 2); i++)
		GameBord[i]->Update();
}

void GameScene::PreRender()
{

}

void GameScene::Render()
{
	for (float i = 0; i < pow(MAX_SIZE, 2); i++)
		GameBord[i]->Render();
}

void GameScene::PostRender()
{
}

void GameScene::GUIRender()
{
}

void GameScene::Swap(float a2)
{
	Moving = true;

	Swap1 = GameBord[nowPos]->GetLocalPosition();
	Swap2 = GameBord[a2]->GetLocalPosition();
	Sw1 = nowPos;
	Sw2 = a2;
}

void GameScene::Move()
{
	if (!Moving) {
		SwapSpeed = 0.001;
		ChackMove();
	}
	else {
		// 이동 방향 벡터
		Vector3 moveDirection1 = XMVector3Normalize(Swap2 - GameBord[nowPos]->GetLocalPosition());
		GameBord[nowPos]->SetLocalPosition(GameBord[nowPos]->GetLocalPosition() + (moveDirection1 * SwapSpeed));

		Vector3 moveDirection2 = XMVector3Normalize(Swap1 - GameBord[Sw2]->GetLocalPosition());
		GameBord[Sw2]->SetLocalPosition(GameBord[Sw2]->GetLocalPosition() + (moveDirection2 * SwapSpeed));

		// Moving
		if(SwapSpeed > 0.00005)
			SwapSpeed -= SwapSpeed * DELTA * 1.9;
		// Move done
		else {
			GameBord[nowPos]->SetLocalPosition(Swap2);
			GameBord[Sw2]->SetLocalPosition(Swap1);

			GameBox* def = GameBord[nowPos];
			GameBord[nowPos] = GameBord[Sw2];
			GameBord[Sw2] = def;
			nowPos = Sw2;
			Moving = false;
		}
	}
}

void GameScene::ChackMove()
{
	int Key_input;
	if		(KEY->Press('W') && nowPos - MAX_SIZE >= 0)							Swap(nowPos - MAX_SIZE);
	else if (KEY->Press('S') && nowPos + MAX_SIZE < pow(MAX_SIZE,2))			Swap(nowPos + MAX_SIZE);
	else if (KEY->Press('A') && ((int)nowPos % (int)MAX_SIZE) - 1 >= 0)			Swap(nowPos - 1);
	else if (KEY->Press('D') && ((int)nowPos % (int)MAX_SIZE) + 1 < MAX_SIZE)	Swap(nowPos + 1);
}

// 총 10번 섞임.
void GameScene::BoxMIX()

{    // 시드값을 얻어서 초기화
	random_device rd;
	mt19937 generator(rd()); // 메르센 트위스터 엔진을 사용
	// 난수 범위를 설정
	uniform_int_distribution<int> distribution(0, 3); // 0부터 3까지의 정수를 생성
	 
	for (int i = 0; i < 10; i++) 
	{
		bool move = false;
		switch (distribution(generator))
		{
		case 0:
			if (nowPos - MAX_SIZE >= 0) {
				Swap(nowPos - MAX_SIZE);
				move = true;
			}
			else i--;
			break;
		case 1:
			if (nowPos + MAX_SIZE < pow(MAX_SIZE, 2)) {
				Swap(nowPos + MAX_SIZE);
				move = true;
			}
			else i--;
			break;
		case 2:
			if (((int)nowPos % (int)MAX_SIZE) - 1 >= 0) {
				Swap(nowPos - 1);
				move = true;
			}
			else i--;
			break;
		case 3:
			if (((int)nowPos % (int)MAX_SIZE) + 1 < MAX_SIZE) {
				Swap(nowPos + 1);
				move = true;
			}
			else i--;
			break;
		}

		if (move) {
			GameBord[nowPos]->SetLocalPosition(Swap2);
			GameBord[Sw2]->SetLocalPosition(Swap1);

			GameBox* def = GameBord[nowPos];
			GameBord[nowPos] = GameBord[Sw2];
			GameBord[Sw2] = def;
			nowPos = Sw2;
		}
	}
}