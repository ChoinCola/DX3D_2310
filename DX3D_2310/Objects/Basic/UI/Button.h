#pragma once


// Quad 클래스를 상속하는 Button 클래스를 정의합니다.
class Button : public Quad
{
protected:
	enum State
	{
		// 버튼의 상태를 정의한 열거형입니다.
		// NONE: 기본 상태
		// DOWN: 버튼이 눌렸을 때의 상태
		// OVER: 마우스 포인터가 버튼 위에 있을 때의 상태
		NONE, DOWN, OVER
	};

	// 각 버튼 상태에 대한 색상을 정의한 상수 멤버 변수입니다.
	const Float4 NONE_COLOR = { 1.0f, 1.0f, 1.0f, 1.0f }; // 기본 상태의 색상 (흰색)
	const Float4 DOWN_COLOR = { 0.5f, 0.5f, 0.5f, 0.5f }; // 눌린 상태의 색상 (회색)
	const Float4 OVER_COLOR = { 0.9f, 0.9f, 0.9f, 0.9f }; // 마우스 포인터가 위에 있을 때의 색상 (연한 회색)

public:
	// 버튼 생성자 함수입니다.
	Button(wstring textureFile);

	// 버튼 생성자 함수 (크기 지정)입니다.
	Button(Float2 size = { 32, 32 });

	// 버튼 소멸자 함수입니다.
	~Button() = default;

	// 버튼의 상태를 업데이트하는 함수입니다.
	void Update();

	// 버튼의 클릭 이벤트를 설정하는 함수입니다.
	void SetEvent(Event event) { this->event = event; }

	// 버튼의 클릭 이벤트 및 이벤트 핸들러 객체를 설정하는 함수입니다.
	void SetParamEvnet(ParamEvent paramevent, void* object)
	{
		this->paramevent = paramevent;
		this->object = object;
	}

	// 버튼의 놓아짐 이벤트를 설정하는 함수입니다.
	void SetUpEvent(Event event) { this->Upevent = event; }

	// 버튼의 놓아짐 이벤트 및 이벤트 핸들러 객체를 설정하는 함수입니다.
	void SetUpParamEvent(ParamEvent paramevent, void* object)
	{
		this->Upparamevent = paramevent;
		this->Upobject = object;
	}


	// 버튼의 올려졌을때 이벤트를 설정하는 함수입니다.
	void SetOverEvent(Event event) { this->Overevent = event; }

	// 버튼의 올려졌을때 이벤트 및 이벤트 핸들러 객체를 설정하는 함수입니다.
	void SetOverParamEvent(ParamEvent paramevent, void* object)
	{
		this->Overparamevent = paramevent;
		this->Overobject = object;
	}


	// 현재 버튼의 상태를 반환하는 함수입니다.
	State GetState() { return state; }

private:
	// 버튼의 상태를 확인하고 이벤트를 실행하는 내부 함수입니다.
	void ChackStateEvent();

	// 지정된 이벤트 및 이벤트 핸들러 객체를 실행하는 내부 함수입니다.
	void RunEvent(Event ev, ParamEvent pev, void* input);

protected:
	State state = NONE; // 현재 버튼의 상태

	Event event = nullptr; // 클릭 이벤트 핸들러 함수 포인터
	ParamEvent paramevent = nullptr; // 클릭 이벤트 핸들러 객체 포인터
	void* object = nullptr; // 클릭 이벤트 핸들러 객체

	Event Overevent = nullptr; // 위에 올려졌을때 이벤트 핸들러 함수 포인터
	ParamEvent Overparamevent = nullptr; // 위에 올려졌을때 이벤트 핸들러 객체 포인터
	void* Overobject = nullptr; // 위에 올려졌을때 이벤트 핸들러 객체

	Event Upevent = nullptr; // 놓아짐 이벤트 핸들러 함수 포인터
	ParamEvent Upparamevent = nullptr; // 놓아짐 이벤트 핸들러 객체 포인터
	void* Upobject = nullptr; // 놓아짐 이벤트 핸들러 객체

	bool isDownCheck = false; // 버튼이 눌렸는지 여부를 나타내는 플래그
	bool ChackHudprint = false;
};