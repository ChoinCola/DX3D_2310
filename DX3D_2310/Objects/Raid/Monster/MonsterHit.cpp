#include "Framework.h"
#include "MonsterHit.h"

MonsterHit::MonsterHit(TopViewMonster* monster)
	: MonsterAction(monster)
{
}

void MonsterHit::Update()
{
	if (HitTime > MAXHITDELAY) {
		monster->SetAction(TopViewMonster::TRACE);
		HitTime = 0;
	}

	HitTime += DELTA;
	monster->SetColor(Float4(1, 0, 0, 1));
}

void MonsterHit::Start()
{
	monster->Hit(10);
}
