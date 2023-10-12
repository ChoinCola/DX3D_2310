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

	{// ����
		Vector3 def = GetLocalRotation();
		if (def.y >= 360)
			def.y = 0;

		def.y += DELTA * 0.00915;
		SetLocalRotation(def);
	}

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
