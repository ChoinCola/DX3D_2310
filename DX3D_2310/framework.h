#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             

#define WIN_START_X 100
#define WIN_START_Y	100

#define WIN_WIDTH 1280
#define WIN_HEIGHT 720

#include <windows.h>
#include <string>

#include <vector>	// 연결 컨테이너

#include <map>
#include <unordered_map> // 비선형 자료구조중 정렬이 필요없을 때 사용.

#include <d3d11.h> // 다이렉트X선언
#include <d3dcompiler.h>
#include <DirectXMath.h>
// Math를 사용하는 이유
// 내부구조가 매우 어렵게 되어있다. vector처럼 구성되어있다.
// 대신 내부적으로 속도가 좀 빠르다.
// MSD에서 추천하는 라이브러리이다.

#pragma comment(lib, "d3d11.lib") // 라이브러리 추가
#pragma comment(lib, "d3dcompiler.lib")

using namespace std;
using namespace DirectX;

// Framework Header
#include "Framework/Utilities/Singleton.h"

#include "Framework/Device/Device.h"

#define DEVICE Device::Get()->GetDevice()
#define DC     Device::Get()->GetDC()

// Buffer Set
#include "Convertor/Buffer.h"

// Shader Set
#include "Convertor/Shader.h"

// Obejct Header
#include "Object/IObject.h"
#include "Object/Box.h"

// Scene
#include "Scenes/Scene.h"
#include "Manager/GameManager.h"

//extern 전역변수가 있다는걸 알려주는 용도
extern HWND hWnd;