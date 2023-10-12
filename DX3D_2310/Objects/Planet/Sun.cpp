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
	{// ����
		Vector3 def = GetLocalRotation();
		if (def.y >= 360)
			def.y = 0;

		// �¾� �����ֱ� �� 25�Ͽ��� 35��.
		// �������� �� 28������ ����
		// �ʹ� �����ϱ� ���� �����ֱ��� 2.8�� �������ϸ��
		def.y += DELTA * 0.089;
		SetLocalRotation(def);
	}

	{// ����
		// �¾��� ������ �ϱ�� ������ ���⼱ �������� ����.
	}
	Spher::UpdateWorld();
}

void Sun::Render()
{
	Spher::Render();
}
