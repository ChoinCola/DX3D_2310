#include "Framework.h"

Moon::Moon(Transform* Parent)
	:	Spher(1.7, 4)
{
	parent = Parent;
}

Moon::~Moon()
{
}

void Moon::Update()
{

	{// 자전
		Vector3 def = GetLocalRotation();
		if (def.y >= 360)
			def.y = 0;

		def.y += DELTA * 0.00915;
		SetLocalRotation(def);
	}

	{// 공전
		// position이 지구와 태양거리 기준에서 
		// X = sin() Z = cos()가 되어야함.
		// 공전각도 = 360도로 분할해야함으로.
		// 2 * PI * (태양의 반지름 + 지구와 태양의 거리 + 지구의 반지름) = 원의 둘레
		// 2 * 3.141592 * (149,597,870.7 + 696,340 + 6,371)
		//  = 98,410,882.73...이 나옴.  대충 반올림해서 98로 잡음.
		// 공전속도로 나누면 3,306,817
		// 360 으로 백분율 = 0.000108866... 이 나옴.
		// 사실 위에꺼 계산할필요 없이 1년 = 365일. 360도기준으로 약 0.99도 공전각도가 나옴
		// 계산

		Vector3 def = GetLocalPosition();

		def.x = sin(AngleRevolution) * 9.8;
		def.z = cos(AngleRevolution) * 9.8;
		def += parent->GetLocalPosition();

		if (AngleRevolution <= 360) AngleRevolution += DELTA * 0.5;
		else AngleRevolution = 0;
		SetLocalPosition(def);
	}
	Spher::UpdateWorld();
}

void Moon::Render()
{
	Spher::Render();
}
