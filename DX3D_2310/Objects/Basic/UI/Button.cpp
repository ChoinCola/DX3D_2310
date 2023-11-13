#include "Framework.h"

// ��ư�� ������ �Լ� ����
Button::Button(wstring textureFile) : Quad(textureFile)
{
	// Quad Ŭ������ �����ڸ� ȣ���ϰ� �ؽ�ó ������ �����մϴ�.
}

// ��ư�� ������ �Լ� (ũ�� ����) ����
Button::Button(Float2 size) : Quad(size)
{
	// Quad Ŭ������ �����ڸ� ȣ���ϰ� ũ�⸦ �����մϴ�.
}

// ��ư�� ������Ʈ �Լ� ����
void Button::Update()
{
	// ��ư�� Ȱ��ȭ���� �ʾ��� ��� �ƹ� ���۵� �������� �ʽ��ϴ�.
	if (IsActive()) return;

	// ���콺 Ŀ������ �浹�� üũ�մϴ�.
	if (CollisionChack(Mouse::Get()->GetPosition()))
	{
		// ���콺 ���� ��ư�� ������ ��, ���� ���� �÷��׸� �����մϴ�.
		if (Mouse::Get()->Down(0))
			isDownCheck = true;

		// ���콺 ���� ��ư�� ���� ������ ��� DOWN ���·� �����ϰ�,
		// �׷��� ���� ��� OVER ���·� �����մϴ�.
		if (Mouse::Get()->Press(0))
			state = DOWN;
		else
			state = OVER;

		// ��ư�� ���� ���¿��� ���콺 ���� ��ư�� ������ ��, Ŭ�� �̺�Ʈ�� �����մϴ�.
		if (isDownCheck && Mouse::Get()->Up(0))
		{
			// �̺�Ʈ�� �����Ǿ� ������ �����մϴ�.
			if (event)
				event();

			// ������ �̺�Ʈ �� �̺�Ʈ �ڵ鷯 ��ü�� �����մϴ�.
			RunEvent(Upevent, Upparamevent, Upobject);

			// ���� ���� �÷��׸� �����մϴ�.
			isDownCheck = false;
		}
	}
	else
	{
		// ���콺 Ŀ���� ��ư ���� ���� ��� NONE ���·� �����մϴ�.
		state = NONE;

		// ���콺 ���� ��ư�� ������ ��, ���� ���� �÷��׸� �����մϴ�.
		if (Mouse::Get()->Up(0)) {
			isDownCheck = false;
		}
	}

	// ��ư�� ���¿� ���� ������ ������ �����մϴ�.
	switch (state)
	{
	case Button::NONE:
		material->GetBuffer()->diffuse = NONE_COLOR; // �⺻ ������ ����
		break;
	case Button::DOWN:
		material->GetBuffer()->diffuse = DOWN_COLOR; // ���� ������ ����
		break;
	case Button::OVER:
		material->GetBuffer()->diffuse = OVER_COLOR; // ���콺 �����Ͱ� ���� ���� ���� ����
		break;
	}

	// ��ư�� ���¿� ���� �̺�Ʈ�� üũ�ϰ� �����ϴ� �Լ��� ȣ���մϴ�.
	ChackStateEvent();

	// ��ư�� ���� ��ȯ�� ������Ʈ�մϴ�.
	UpdateWorld();
}

// ��ư�� ���¿� ���� �̺�Ʈ�� üũ�ϰ� �����ϴ� �Լ� ����
void Button::ChackStateEvent()
{
	switch (state)
	{
	case Button::NONE:
		break;
	case Button::DOWN:
		// DOWN ���¿����� Ŭ�� �̺�Ʈ�� �����մϴ�.
		RunEvent(event, paramevent, object);
		break;
	case Button::OVER:
		break;
	default:
		break;
	}
}

// ������ �̺�Ʈ �� �̺�Ʈ �ڵ鷯 ��ü�� �����ϴ� �Լ� ����
void Button::RunEvent(Event ev, ParamEvent pev, void* input)
{
	// �̺�Ʈ �Լ��� �����Ǿ� ������ �����մϴ�.
	if (ev != nullptr) ev();
	// �̺�Ʈ �ڵ鷯 �Լ��� ��ü�� �����Ǿ� ������ �����մϴ�.
	if (pev != nullptr && input != nullptr) pev(input);
}