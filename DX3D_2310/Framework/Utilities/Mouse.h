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

	// ������ ȭ���� n�� �̻��ϰ�� ���콺�� ������ handle�� ����.
	void SetHandle(HWND handle)
	{
		this->handle = handle;
	}

	void Update();

	// ���콺�� ��ư�� True��, 
	// ���콺�� ��ġ��ǥ�� �����(MOUSEMOVE)
	// Winmain �������� ���콺 Position��ġ�� ��ȯ��.
	// �ٹ�ư�� state�� ��ȯ�Ѵ�.
	LRESULT InputProc(UINT message, WPARAM wParam, LPARAM lParam);

	// ���콺�� ���� Position ��ȯ
	Vector3 GetPosition() { return position; }
	Vector3* GetPositionOrigin() { return &position; }


	Vector3 GetDelta() { return delta; }
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

	HWND handle;	// �������ڵ�
	Vector3 Oldposition; // ���콺 ������ġ
	Vector3 position;	// ���콺 ���� ��ġ
	Vector3 delta;
	POINT clientCenterPos = { WIN_WIDTH >> 1, WIN_HEIGHT >> 1 };

	// ��ư�� ����, ������, ������
	byte buttonStatus[MAX_INPUT_MOUSE];
	byte buttonOldStatus[MAX_INPUT_MOUSE];
	byte buttonMap[MAX_INPUT_MOUSE];

	// ���콺 Ŀ���� �ٹ�ư ����
	Vector3 CusorStatus;
	Vector3 CusorOldStatus;
	Vector3 CusorMoveValue;

	// ����Ŭ�� �˻縦 ���� ����
	DWORD timeDblClk;
	DWORD startDblClk[MAX_INPUT_MOUSE];
	int buttonCount[MAX_INPUT_MOUSE];

	bool IsSetMouseHold = false;
	bool MouseHoldMod = true;
};