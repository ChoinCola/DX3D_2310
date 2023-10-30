#include "Framework.h"


Mouse::Mouse()
{
	// ���콺 �⺻��ǥ ����
	position = Vector3(0, 0, 0);

	// ���콺 ��ġ��ǥ �⺻����
	CusorStatus =		Vector3(0.0f, 0.0f, 0.0f);
	CusorOldStatus =	Vector3(0.0f, 0.0f, 0.0f);
	// ��ֶ����� ��
	CusorMoveValue =	Vector3(0.0f, 0.0f, 0.0f);

	// ��� ���콺 State�ʱ�ȭ.
	ZeroMemory(buttonStatus, sizeof(byte) * MAX_INPUT_MOUSE);
	ZeroMemory(buttonOldStatus, sizeof(byte) * MAX_INPUT_MOUSE);
	ZeroMemory(buttonMap, sizeof(byte) * MAX_INPUT_MOUSE);

	ZeroMemory(startDblClk, sizeof(DWORD) * MAX_INPUT_MOUSE);
	ZeroMemory(buttonCount, sizeof(int) * MAX_INPUT_MOUSE);

	// ����Ŭ���� ����Ŭ���ð� ��ȯ (����Ŭ�� ó��- �ι�° �� �ɸ��ð�)
	timeDblClk = GetDoubleClickTime();
	// ����ð� ��ȯ
	startDblClk[0] = GetTickCount();

	// ����Ŭ�� ���۽ð� �ʱ�ȭ
	for (int i = 1; i < MAX_INPUT_MOUSE; i++)
		startDblClk[i] = startDblClk[0];

	// SPI�ý��� �Ű������� �� ��ũ�ѿ� ������ ��ũ�ѵǴ��� ���� ��ȯ.
	DWORD tLine = 0;
	SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &tLine, 0);
}

Mouse::~Mouse()
{
}


void Mouse::Update()
{
	// ���콺 OldStatus�� ���� Status�� ���ŵ�.
	memcpy(buttonOldStatus, buttonStatus, sizeof(buttonOldStatus));

	// buttonMap�� Sattus�ʱ�ȭ
	ZeroMemory(buttonStatus, sizeof(buttonStatus));
	ZeroMemory(buttonMap, sizeof(buttonMap));

	// ��ư���°��� Key���� �޾� �ʱ�ȭ��.
	buttonStatus[0] = GetAsyncKeyState(VK_LBUTTON) & 0x8000 ? 1 : 0;
	buttonStatus[1] = GetAsyncKeyState(VK_RBUTTON) & 0x8000 ? 1 : 0;
	buttonStatus[2] = GetAsyncKeyState(VK_MBUTTON) & 0x8000 ? 1 : 0;

	// ��ư���°��� ��ȯ�Ͽ� �˻�
	for (DWORD i = 0; i < MAX_INPUT_MOUSE; i++)
	{
		int tOldStatus = buttonOldStatus[i];
		int tStatus = buttonStatus[i];

		// ���� ��ư���°� ������������ ������������ �ö󰣻������� �˻��Ͽ� ������.
		if (tOldStatus == 0 && tStatus == 1)
			buttonMap[i] = BUTTON_INPUT_STATUS_DOWN;
		else if (tOldStatus == 1 && tStatus == 0)
			buttonMap[i] = BUTTON_INPUT_STATUS_UP;
		else if (tOldStatus == 1 && tStatus == 1)
			buttonMap[i] = BUTTON_INPUT_STATUS_PRESS;
		else
			buttonMap[i] = BUTTON_INPUT_STATUS_NONE;
	}

	// ���� ��ġ���� ���� ����ü ����
	POINT point;
	// ���콺 Ŀ���� ȭ����ġ�� ������
	GetCursorPos(&point);
	// ȭ����ǥ�� Ŭ���̾�Ʈ ������ �����ǥ�� ��ȯ�Ͽ� point�� ������.
	ScreenToClient(handle, &point);
	//�׸��Ͽ� point���� ���콺Ŀ���� Ŭ���̾�Ʈ ������ ��ġ�� �����.

	// ���� Oldstatus���� ������ġ�� ������.
	CusorOldStatus.x = CusorStatus.x;
	CusorOldStatus.y = CusorStatus.y;

	CusorStatus.x = float(point.x);
	CusorStatus.y = float(point.y);

	CusorMoveValue = CusorStatus - CusorOldStatus;

	// ���� ���콺�� Status�� �����Ͽ� ����Ŭ������ �ƴϸ� ��¥ Ŭ������ �˻���.
	// ���콺 ���� ��ư�� ������ ��, 
	// ���� �ð� - ���콺 ����Ŭ���߻� �ð��� ����Ŭ���� �ð����� Ŭ���, 
	// ���� õõ�� ������ ���. ���� State������ button���������� 0���� �ʱ�ȭ.
	// ����Ŭ�� ���� �������� ����.
	// �ѹ��̶� ������ ���. buttonCount == 1�� ���. ����Ŭ�� �ð��� ������.

	// ��ưmap�� ������� ó���Ǿ��� ���.
	//�������°� 1�ϰ��, �Ȱ���. ������ 0���� ó��
	//�ι����ȴٰ� �Ǵܵǰ�, �ö��� ��. �ӵ��� �����ٸ�, ����Ŭ������ ó����.
	//����Ŭ���̴� �ƴϴ� �ϴ� �ö󰡰�, �ι� ��������� 0���� �ʱ�ȭ����.
	DWORD tButtonStatus = GetTickCount();	// ����ð� ��ȯ
	for (DWORD i = 0; i < MAX_INPUT_MOUSE; i++)
	{
		if (buttonMap[i] == BUTTON_INPUT_STATUS_DOWN)
		{
			if (buttonCount[i] == 1)
			{
				if ((tButtonStatus - startDblClk[i]) >= timeDblClk)
					buttonCount[i] = 0;
			}
			buttonCount[i]++;

			if (buttonCount[i] == 1)
				startDblClk[i] = tButtonStatus;
		}

		if (buttonMap[i] == BUTTON_INPUT_STATUS_UP)
		{
			if (buttonCount[i] == 1)
			{
				if ((tButtonStatus - startDblClk[i]) >= timeDblClk)
					buttonCount[i] = 0;
			}
			else if (buttonCount[i] == 2)
			{
				if ((tButtonStatus - startDblClk[i]) <= timeDblClk)
					buttonMap[i] = BUTTON_INPUT_STATUS_DBLCLK;

				buttonCount[i] = 0;
			}
		}
	}

	// F2�� ������ ���콺 ��ġ�� �߾ӿ� �����Ѵ�.
	SetMouseHold();
	if (IsSetMouseHold) {
		// ȭ�� �߾� ��ǥ ���
		int centerX = GetSystemMetrics(SM_CXSCREEN) * 0.5;
		int centerY = GetSystemMetrics(SM_CYSCREEN) * 0.5;
		SetCursorPos(centerX, centerY);
	}
}

LRESULT Mouse::InputProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// �Ż����߻���, LBUTTONDOWN �Ǵ� ������ ó�� �Ǿ��� ���.
	// ���콺�� POSITION�� �����Ѵ�.

	if (message == WM_LBUTTONDOWN || message == WM_MOUSEMOVE)
	{
		position.x = (float)LOWORD(lParam);
		position.y = (float)(WIN_HEIGHT - HIWORD(lParam));
	}

	// ���콺 ���� �����Ͽ��� ��. ó���Ѵ�.
	if (message == WM_MOUSEWHEEL)
	{
		// �޾ƿ� �޼��� ó���� �����Ͱ��� ��� �Ǵ� ���������� ������ó����
		// �����Ͽ� wParam�� ó���Ѵ�.
		short tCusorValue = (short)HIWORD(wParam);

		// ���콺 Ŀ���� z���� �ٵ����� ó���� ����Ѵ�.
		// ���� ��ġ���� �����ϰ�. ���� ���� �����Ͽ� �����Ѵ�.
		CusorOldStatus.z = CusorStatus.z;
		CusorStatus.z += (float)tCusorValue;
	}
	return TRUE;
}

void Mouse::SetMouseHold()
{
	if (KEY->Down(VK_F2)) {
		// ���콺 Ȧ�忩�θ� ǥ��
		IsSetMouseHold = !IsSetMouseHold;
		// ���콺 ���� ó��
		ShowCursor(!IsSetMouseHold);
	}
}