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
	{// ����
		Vector3 def = GetLocalRotation();
		if (def.y >= 360)
			def.y = 0;

		// ���� 24�ð��� 1�� ȸ����.
		// �ð��� 15���� ȸ����
		// 1�п� 25������ ȸ��
		// 1�ʿ� 1�и�ŭ ȸ���ϵ��� ����
		def.y += DELTA * 0.25;
		SetLocalRotation(def);
	}

	/*
		������ �¾��� �Ÿ� 149,597,870.7km
		14.9 �� �����.
	*/

	/*
		�¾��� ������ 696,340km
	*/

	/*
		������ ������ 6,371km
		6.3���� �����.
	*/

	/*
		���� �����ӵ� 29.76km/s

	*/

	{// ����
		// position�� ������ �¾�Ÿ� ���ؿ��� 
		// X = sin() Z = cos()�� �Ǿ����.
		// �������� = 360���� �����ؾ�������.
		// 2 * PI * (�¾��� ������ + ������ �¾��� �Ÿ� + ������ ������) = ���� �ѷ�
		// 2 * 3.141592 * (149,597,870.7 + 696,340 + 6,371)
		//  = 98,410,882.73...�� ����.  ���� �ݿø��ؼ� 98�� ����.
		// �����ӵ��� ������ 3,306,817
		// 360 ���� ����� = 0.000108866... �� ����.
		// ��� ������ ������ʿ� ���� 1�� = 365��. 360���������� �� 0.99�� ���������� ����
		// ���
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
