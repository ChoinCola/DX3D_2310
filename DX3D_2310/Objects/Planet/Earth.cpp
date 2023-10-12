#include "Framework.h"

Ealth::Ealth(Vector3 pivot)
	:	Spher(6.3, 4)
{
	this->SetPivot(pivot);

}

Ealth::~Ealth()
{
}

void Ealth::Update()
{
	{// 자전
		Vector3 def = GetLocalRotation();
		if (def.y >= 360)
			def.y = 0;

		// 지구 24시간에 1번 회전함.
		// 시간당 15도를 회전함
		// 1분에 25도정도 회전
		// 1초에 1분만큼 회전하도록 기입
		def.y += DELTA * 0.25;
		SetLocalRotation(def);
	}

	/*
		지구와 태양의 거리 149,597,870.7km
		14.9 로 축소함.
	*/

	/*
		태양의 반지름 696,340km
	*/

	/*
		지구의 반지름 6,371km
		6.3으로 축소함.
	*/

	/*
		지구 공전속도 29.76km/s

	*/

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
		def.x = sin(AngleRevolution) * 98;
		def.z = cos(AngleRevolution) * 98;


		if (AngleRevolution <= 360) AngleRevolution += DELTA * 0.0998;
		else AngleRevolution = 0;
		SetLocalPosition(def);
	}
	Spher::UpdateWorld();
}

void Ealth::Render()
{
	Spher::Render();
}
