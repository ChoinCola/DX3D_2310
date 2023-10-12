#include "Framework.h"
#include "Sun.h"

Sun::Sun()
	:	Spher(30, 3)
{

}

Sun::~Sun()
{
}

void Sun::Update()
{
	{// 자전
		Vector3 def = GetLocalRotation();
		if (def.y >= 360)
			def.y = 0;

		// 태양 자전주기 약 25일에서 35일.
		// 지구보다 약 28배정도 느림
		// 너무 느리니까 지구 자전주기의 2.8배 느리게하면됨
		def.y += DELTA * 0.089;
		SetLocalRotation(def);
	}

	{// 공전
		// 태양은 공전을 하기는 하지만 여기선 구현하지 않음.
	}
	Spher::UpdateWorld();
}

void Sun::Render()
{
	Spher::Render();
}
