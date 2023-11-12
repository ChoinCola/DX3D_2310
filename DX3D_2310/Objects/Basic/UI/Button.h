#pragma once

class Button : public Quad
{
protected:
	enum State
	{
	//	�⺻	  �ٿ�	���콺 �����Ͱ� ���� ������
		NONE, DOWN, OVER
	};

	const Float4 NONE_COLOR = { 1.0f, 1.0f, 1.0f, 1.0f };
	const Float4 DOWN_COLOR = { 0.5f, 0.5f, 0.5f, 0.5f };
	const Float4 OVER_COLOR = { 0.9f, 0.9f, 0.9f, 0.9f };

public:
	Button(wstring textureFile);
	Button(Float2 size = {32, 32});
	~Button() = default;

	void Update();

	void SetEvent(Event event) { this->event = event; }
	void SetPramEvnet(ParamEvent paramevet, void* object)
	{
		this->paramevent = paramevet;
		this->object = object;
	}

	void SetUpEvent(Event event) { this->Upevent = event; }
	void SetUpPramEvnet(ParamEvent paramevet, void* object)
	{
		this->Upparamevent = paramevet;
		this->Upobject = object;
	}


	State GetState() { return state; }

private:
	void ChackStateEevent();
	void RunEvent(Event ev, ParamEvent pev, void* input);

protected:
	State state = NONE;

	Event event = nullptr;
	ParamEvent paramevent = nullptr;
	void* object = nullptr;

	Event Upevent = nullptr;
	ParamEvent Upparamevent = nullptr;
	void* Upobject = nullptr;


	bool isDownCheck = false;
};