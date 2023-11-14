#pragma once


// Quad Ŭ������ ����ϴ� Button Ŭ������ �����մϴ�.
class Button : public Quad
{
protected:
	enum State
	{
		// ��ư�� ���¸� ������ �������Դϴ�.
		// NONE: �⺻ ����
		// DOWN: ��ư�� ������ ���� ����
		// OVER: ���콺 �����Ͱ� ��ư ���� ���� ���� ����
		NONE, DOWN, OVER
	};

	// �� ��ư ���¿� ���� ������ ������ ��� ��� �����Դϴ�.
	const Float4 NONE_COLOR = { 1.0f, 1.0f, 1.0f, 1.0f }; // �⺻ ������ ���� (���)
	const Float4 DOWN_COLOR = { 0.5f, 0.5f, 0.5f, 0.5f }; // ���� ������ ���� (ȸ��)
	const Float4 OVER_COLOR = { 0.9f, 0.9f, 0.9f, 0.9f }; // ���콺 �����Ͱ� ���� ���� ���� ���� (���� ȸ��)

public:
	// ��ư ������ �Լ��Դϴ�.
	Button(wstring textureFile);

	// ��ư ������ �Լ� (ũ�� ����)�Դϴ�.
	Button(Float2 size = { 32, 32 });

	// ��ư �Ҹ��� �Լ��Դϴ�.
	~Button() = default;

	// ��ư�� ���¸� ������Ʈ�ϴ� �Լ��Դϴ�.
	void Update();

	// ��ư�� Ŭ�� �̺�Ʈ�� �����ϴ� �Լ��Դϴ�.
	void SetEvent(Event event) { this->event = event; }

	// ��ư�� Ŭ�� �̺�Ʈ �� �̺�Ʈ �ڵ鷯 ��ü�� �����ϴ� �Լ��Դϴ�.
	void SetParamEvnet(ParamEvent paramevent, void* object)
	{
		this->paramevent = paramevent;
		this->object = object;
	}

	// ��ư�� ������ �̺�Ʈ�� �����ϴ� �Լ��Դϴ�.
	void SetUpEvent(Event event) { this->Upevent = event; }

	// ��ư�� ������ �̺�Ʈ �� �̺�Ʈ �ڵ鷯 ��ü�� �����ϴ� �Լ��Դϴ�.
	void SetUpParamEvent(ParamEvent paramevent, void* object)
	{
		this->Upparamevent = paramevent;
		this->Upobject = object;
	}


	// ��ư�� �÷������� �̺�Ʈ�� �����ϴ� �Լ��Դϴ�.
	void SetOverEvent(Event event) { this->Overevent = event; }

	// ��ư�� �÷������� �̺�Ʈ �� �̺�Ʈ �ڵ鷯 ��ü�� �����ϴ� �Լ��Դϴ�.
	void SetOverParamEvent(ParamEvent paramevent, void* object)
	{
		this->Overparamevent = paramevent;
		this->Overobject = object;
	}


	// ���� ��ư�� ���¸� ��ȯ�ϴ� �Լ��Դϴ�.
	State GetState() { return state; }

private:
	// ��ư�� ���¸� Ȯ���ϰ� �̺�Ʈ�� �����ϴ� ���� �Լ��Դϴ�.
	void ChackStateEvent();

	// ������ �̺�Ʈ �� �̺�Ʈ �ڵ鷯 ��ü�� �����ϴ� ���� �Լ��Դϴ�.
	void RunEvent(Event ev, ParamEvent pev, void* input);

protected:
	State state = NONE; // ���� ��ư�� ����

	Event event = nullptr; // Ŭ�� �̺�Ʈ �ڵ鷯 �Լ� ������
	ParamEvent paramevent = nullptr; // Ŭ�� �̺�Ʈ �ڵ鷯 ��ü ������
	void* object = nullptr; // Ŭ�� �̺�Ʈ �ڵ鷯 ��ü

	Event Overevent = nullptr; // ���� �÷������� �̺�Ʈ �ڵ鷯 �Լ� ������
	ParamEvent Overparamevent = nullptr; // ���� �÷������� �̺�Ʈ �ڵ鷯 ��ü ������
	void* Overobject = nullptr; // ���� �÷������� �̺�Ʈ �ڵ鷯 ��ü

	Event Upevent = nullptr; // ������ �̺�Ʈ �ڵ鷯 �Լ� ������
	ParamEvent Upparamevent = nullptr; // ������ �̺�Ʈ �ڵ鷯 ��ü ������
	void* Upobject = nullptr; // ������ �̺�Ʈ �ڵ鷯 ��ü

	bool isDownCheck = false; // ��ư�� ���ȴ��� ���θ� ��Ÿ���� �÷���
	bool ChackHudprint = false;
};