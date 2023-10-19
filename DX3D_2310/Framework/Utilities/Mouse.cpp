#include "Framework.h"


Mouse::Mouse()
{
	// 마우스 기본좌표 설정
	position = Vector3(0, 0, 0);

	// 마우스 위치좌표 기본설정
	CusorStatus =		Vector3(0.0f, 0.0f, 0.0f);
	CusorOldStatus =	Vector3(0.0f, 0.0f, 0.0f);
	// 노멀라이즈 값
	CusorMoveValue =	Vector3(0.0f, 0.0f, 0.0f);

	// 모든 마우스 State초기화.
	ZeroMemory(buttonStatus, sizeof(byte) * MAX_INPUT_MOUSE);
	ZeroMemory(buttonOldStatus, sizeof(byte) * MAX_INPUT_MOUSE);
	ZeroMemory(buttonMap, sizeof(byte) * MAX_INPUT_MOUSE);

	ZeroMemory(startDblClk, sizeof(DWORD) * MAX_INPUT_MOUSE);
	ZeroMemory(buttonCount, sizeof(int) * MAX_INPUT_MOUSE);

	// 더블클릭시 더블클릭시간 반환 (더블클릭 처음- 두번째 의 걸린시간)
	timeDblClk = GetDoubleClickTime();
	// 현재시간 반환
	startDblClk[0] = GetTickCount();

	// 더블클릭 시작시간 초기화
	for (int i = 1; i < MAX_INPUT_MOUSE; i++)
		startDblClk[i] = startDblClk[0];

	// SPI시스템 매개변수로 휠 스크롤에 몃줄이 스크롤되는지 정보 반환.
	DWORD tLine = 0;
	SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &tLine, 0);
}

Mouse::~Mouse()
{
}


void Mouse::Update()
{
	// 마우스 OldStatus에 현재 Status가 갱신됨.
	memcpy(buttonOldStatus, buttonStatus, sizeof(buttonOldStatus));

	// buttonMap과 Sattus초기화
	ZeroMemory(buttonStatus, sizeof(buttonStatus));
	ZeroMemory(buttonMap, sizeof(buttonMap));

	// 버튼상태값을 Key값을 받아 초기화함.
	buttonStatus[0] = GetAsyncKeyState(VK_LBUTTON) & 0x8000 ? 1 : 0;
	buttonStatus[1] = GetAsyncKeyState(VK_RBUTTON) & 0x8000 ? 1 : 0;
	buttonStatus[2] = GetAsyncKeyState(VK_MBUTTON) & 0x8000 ? 1 : 0;

	// 버튼상태값을 순환하여 검사
	for (DWORD i = 0; i < MAX_INPUT_MOUSE; i++)
	{
		int tOldStatus = buttonOldStatus[i];
		int tStatus = buttonStatus[i];

		// 현재 버튼상태가 눌린상태인지 누른상태인지 올라간상태인지 검사하여 기입함.
		if (tOldStatus == 0 && tStatus == 1)
			buttonMap[i] = BUTTON_INPUT_STATUS_DOWN;
		else if (tOldStatus == 1 && tStatus == 0)
			buttonMap[i] = BUTTON_INPUT_STATUS_UP;
		else if (tOldStatus == 1 && tStatus == 1)
			buttonMap[i] = BUTTON_INPUT_STATUS_PRESS;
		else
			buttonMap[i] = BUTTON_INPUT_STATUS_NONE;
	}

	// 현재 위치값을 넣을 구조체 선언
	POINT point;
	// 마우스 커서의 화면위치를 가져옴
	GetCursorPos(&point);
	// 화면좌표를 클라이언트 영역의 상대좌표로 변환하여 point에 저장함.
	ScreenToClient(handle, &point);
	//그리하여 point에는 마우스커서의 클라이언트 영역내 위치가 저장됨.

	// 휠의 Oldstatus값을 현재위치로 갱신함.
	CusorOldStatus.x = CusorStatus.x;
	CusorOldStatus.y = CusorStatus.y;

	CusorStatus.x = float(point.x);
	CusorStatus.y = float(point.y);

	CusorMoveValue = CusorStatus - CusorOldStatus;

	// 현재 마우스의 Status를 감지하여 더블클릭인지 아니면 진짜 클릭인지 검사함.
	// 마우스 왼쪽 버튼이 눌렸을 때, 
	// 현재 시간 - 마우스 더블클릭발생 시간이 더블클릭한 시간보다 클경우, 
	// 대충 천천히 눌렀을 경우. 현재 State에서의 button눌린정도는 0으로 초기화.
	// 더블클릭 하지 않음으로 간주.
	// 한번이라도 눌렸을 경우. buttonCount == 1인 경우. 더블클릭 시간을 갱신함.

	// 버튼map이 상승으로 처리되었을 경우.
	//눌림상태가 1일경우, 똑같이. 느릴때 0으로 처리
	//두번눌렸다고 판단되고, 올라갔을 대. 속도가 빠르다면, 더블클릭으로 처리함.
	//더블클릭이던 아니던 일단 올라가고, 두번 눌렸을경우 0으로 초기화해줌.
	DWORD tButtonStatus = GetTickCount();	// 현재시간 반환
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

	// F2가 눌리면 마우스 위치를 중앙에 고정한다.
	SetMouseHold();
	if (IsSetMouseHold) {
		RECT clientRect;
		GetClientRect(handle, &clientRect);
		// 클라이언트 좌표를 Rect로 가져옴.
		POINT resetpoint = {
			clientRect.right / 2,
			clientRect.bottom / 2};
		// 상대좌표 기준 화면창의 원점으로 마우스 포인터를 고정시킴.
		ClientToScreen(handle, &resetpoint);
		SetCursorPos(resetpoint.x , resetpoint.y);
	}
}

LRESULT Mouse::InputProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// 매새지발생이, LBUTTONDOWN 또는 움직임 처리 되었을 경우.
	// 마우스의 POSITION을 갱신한다.

	if (message == WM_LBUTTONDOWN || message == WM_MOUSEMOVE)
	{
		position.x = (float)LOWORD(lParam);
		position.y = (float)(WIN_HEIGHT - HIWORD(lParam));
	}

	// 마우스 휠이 동작하였을 때. 처리한다.
	if (message == WM_MOUSEWHEEL)
	{
		// 받아온 메세지 처리의 데이터값이 양수 또는 음수임으로 데이터처리시
		// 참고하여 wParam을 처리한다.
		short tCusorValue = (short)HIWORD(wParam);

		// 마우스 커서의 z축은 휠데이터 처리에 사용한다.
		// 이전 위치값을 갱신하고. 이후 값을 갱신하여 적용한다.
		CusorOldStatus.z = CusorStatus.z;
		CusorStatus.z += (float)tCusorValue;
	}
	return TRUE;
}

void Mouse::SetMouseHold()
{
	if(KEY->Down(VK_F2))
		IsSetMouseHold = !IsSetMouseHold;
}