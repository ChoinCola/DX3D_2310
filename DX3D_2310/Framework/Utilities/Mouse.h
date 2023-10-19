#pragma once
#include "Framework/Math/Vector3.h"

#define MAX_INPUT_MOUSE 8

class Mouse : public Singleton<Mouse>
{
	enum
	{
		MOUSE_ROTATION_NONE = 0,
		MOUSE_ROTATION_LEFT,
		MOUSE_ROTATION_RIGHT
	};

	enum
	{
		BUTTON_INPUT_STATUS_NONE = 0,
		BUTTON_INPUT_STATUS_DOWN,
		BUTTON_INPUT_STATUS_UP,
		BUTTON_INPUT_STATUS_PRESS,
		BUTTON_INPUT_STATUS_DBLCLK
	};

public:
	friend class Singleton<Mouse>;

	// 윈도우 화면이 n개 이상일경우 마우스가 움직일 handle를 설정.
	void SetHandle(HWND handle)
	{
		this->handle = handle;
	}

	void Update();

	// 마우스의 버튼이 True시, 
	// 마우스의 위치좌표가 변경시(MOUSEMOVE)
	// Winmain 내에서의 마우스 Position위치를 반환함.
	// 휠버튼의 state를 반환한다.
	LRESULT InputProc(UINT message, WPARAM wParam, LPARAM lParam);

	// 마우스의 현재 Position 반환
	Vector3 GetPosition() { return position; }

	bool Down(DWORD button)
	{
		return buttonMap[button] == BUTTON_INPUT_STATUS_DOWN;
	}

	bool Up(DWORD button)
	{
		return buttonMap[button] == BUTTON_INPUT_STATUS_UP;
	}

	bool Press(DWORD button)
	{
		return buttonMap[button] == BUTTON_INPUT_STATUS_PRESS;
	}

	bool GetIsSetMouseHold() { return IsSetMouseHold; }

	Vector3 GetMoveValue()
	{
		return CusorMoveValue;
	}
private:
	void SetMouseHold();
private:
	Mouse();
	~Mouse();

	HWND handle;	// 윈도우핸들
	Vector3 Oldposition; // 마우스 이전위치
	Vector3 position;	// 마우스 갱신 위치

	// 버튼의 상태, 전상테, 맵정의
	byte buttonStatus[MAX_INPUT_MOUSE];
	byte buttonOldStatus[MAX_INPUT_MOUSE];
	byte buttonMap[MAX_INPUT_MOUSE];

	// 마우스 커서와 휠버튼 변수
	Vector3 CusorStatus;
	Vector3 CusorOldStatus;
	Vector3 CusorMoveValue;

	// 더블클릭 검사를 위한 변수
	DWORD timeDblClk;
	DWORD startDblClk[MAX_INPUT_MOUSE];
	int buttonCount[MAX_INPUT_MOUSE];

	bool IsSetMouseHold = false;
};