#include "Framework.h"

// 버튼의 생성자 함수 정의
Button::Button(wstring textureFile) : Quad(textureFile)
{
	// Quad 클래스의 생성자를 호출하고 텍스처 파일을 전달합니다.
}

// 버튼의 생성자 함수 (크기 지정) 정의
Button::Button(Float2 size) : Quad(size)
{
	// Quad 클래스의 생성자를 호출하고 크기를 전달합니다.
}

// 버튼의 업데이트 함수 정의
void Button::Update()
{
	// 버튼이 활성화되지 않았을 경우 아무 동작도 수행하지 않습니다.
	if (IsActive()) return;

	// 마우스 커서와의 충돌을 체크합니다.
	if (CollisionChack(Mouse::Get()->GetPosition()))
	{
		// 마우스 왼쪽 버튼이 눌렸을 때, 눌린 상태 플래그를 설정합니다.
		if (Mouse::Get()->Down(0))
			isDownCheck = true;

		// 마우스 왼쪽 버튼이 눌린 상태인 경우 DOWN 상태로 설정하고,
		// 그렇지 않은 경우 OVER 상태로 설정합니다.
		if (Mouse::Get()->Press(0))
			state = DOWN;
		else
			state = OVER;

		// 버튼이 눌린 상태에서 마우스 왼쪽 버튼이 놓아질 때, 클릭 이벤트를 실행합니다.
		if (isDownCheck && Mouse::Get()->Up(0))
		{
			// 이벤트가 설정되어 있으면 실행합니다.
			if (event)
				event();

			// 놓아짐 이벤트 및 이벤트 핸들러 객체를 실행합니다.
			RunEvent(Upevent, Upparamevent, Upobject);

			// 눌린 상태 플래그를 해제합니다.
			isDownCheck = false;
		}
	}
	else
	{
		// 마우스 커서가 버튼 위에 없는 경우 NONE 상태로 설정합니다.
		state = NONE;

		// 마우스 왼쪽 버튼이 놓아질 때, 눌린 상태 플래그를 해제합니다.
		if (Mouse::Get()->Up(0)) {
			isDownCheck = false;
		}
	}

	// 버튼의 상태에 따라 적절한 색상을 설정합니다.
	switch (state)
	{
	case Button::NONE:
		material->GetBuffer()->diffuse = NONE_COLOR; // 기본 상태의 색상
		break;
	case Button::DOWN:
		material->GetBuffer()->diffuse = DOWN_COLOR; // 눌린 상태의 색상
		break;
	case Button::OVER:
		material->GetBuffer()->diffuse = OVER_COLOR; // 마우스 포인터가 위에 있을 때의 색상
		break;
	}

	// 버튼의 상태에 따라 이벤트를 체크하고 실행하는 함수를 호출합니다.
	ChackStateEvent();

	// 버튼의 월드 변환을 업데이트합니다.
	UpdateWorld();
}

// 버튼의 상태에 따라 이벤트를 체크하고 실행하는 함수 정의
void Button::ChackStateEvent()
{
	switch (state)
	{
	case Button::NONE:
		break;
	case Button::DOWN:
		// DOWN 상태에서는 클릭 이벤트를 실행합니다.
		RunEvent(event, paramevent, object);
		break;
	case Button::OVER:
		break;
	default:
		break;
	}
}

// 지정된 이벤트 및 이벤트 핸들러 객체를 실행하는 함수 정의
void Button::RunEvent(Event ev, ParamEvent pev, void* input)
{
	// 이벤트 함수가 설정되어 있으면 실행합니다.
	if (ev != nullptr) ev();
	// 이벤트 핸들러 함수와 객체가 설정되어 있으면 실행합니다.
	if (pev != nullptr && input != nullptr) pev(input);
}