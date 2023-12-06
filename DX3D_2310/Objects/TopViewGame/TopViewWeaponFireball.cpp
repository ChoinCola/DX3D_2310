#include "Framework.h"

TopViewWeaponFireBall::TopViewWeaponFireBall(SkillData data, Transform* player)
	: TopViewWeapon(data, player)
{
	FireBallManager::Get();
}

TopViewWeaponFireBall::~TopViewWeaponFireBall()
{
	FireBallManager::Delete();
}

void TopViewWeaponFireBall::Update()
{
	time += DELTA;
	if (time > data.interval)
	{
		time -= data.interval;

		Transform* monster = MonsterManager::Get()->GetClosestMonster(player->GetLocalPosition());

		if (monster) 
		{
			Vector3 direction = (monster->GetLocalPosition() - player->GetLocalPosition()).GetNormalized();
			FireBallManager::Get()->Fire(player->GetLocalPosition(), direction);
		}


	}
	FireBallManager::Get()->Update();
}

void TopViewWeaponFireBall::Render()
{
	FireBallManager::Get()->Render();
}
