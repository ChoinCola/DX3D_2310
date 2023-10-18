﻿#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN

#define WIN_START_X 100
#define WIN_START_Y 100

#define WIN_WIDTH 1280
#define WIN_HEIGHT 720

#define DEVICE		Device::Get()->GetDevice()
#define DC			Device::Get()->GetDeviceContext()
#define KEY			Keyboard::Get()
#define DELTA		Timer::Get()->GetElapsedTime()
#define DIALOG		ImGuiFileDialog::Instance()

#include <windows.h>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <list>
#include <algorithm>
#include <random>

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")
// VertexBlob의 데이터를 빼오기위한 라이브러리

// DirectXTex
#include <DirectXTex/DirectXTex.h>
#pragma comment(lib, "DirectXTex.lib")

// ImGui
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <ImGuiFileDialog.h>

#pragma comment(lib, "ImGui.lib")

using namespace std;
using namespace DirectX;

typedef XMFLOAT4 Float4;
typedef XMFLOAT3 Float3;
typedef XMFLOAT2 Float2;
typedef XMMATRIX Matrix;
typedef XMVECTOR Vector4;	// 다양한 사용방식이 보장된 vector
typedef XMFLOAT4X4 Float4x4;

// Framework Header
#include "Framework/Utilities/Singleton.h"
#include "Framework/Utilities/Keyboard.h"
#include "Framework/Utilities/Timer.h"
#include "Framework/Device/Device.h"

// Shader Header
#include "Framework/Shader/Shader.h"
#include "Framework/Shader/VertexShader.h"
#include "Framework/Shader/PixelShader.h"


// Buffer Header
#include "Framework/Buffer/VertexBuffer.h"
#include "Framework/Buffer/IndexBuffer.h"
#include "Framework/Buffer/ConstBuffer.h"
#include "Framework/Buffer/GlobalBuffer.h"
#include "Framework/Buffer/VertexLayouts.h"

// Render Header
#include "Framework/Render/Texture.h"
#include "Framework/Render/Material.h"
#include "Framework/Render/Mesh.h"

// Math Header
#include "Framework/Math/Vector3.h"
#include "Framework/Math/Transform.h"

// View Header
#include "Framework/Environment/Environment.h"

// Obejct Header
#include "Objects/Basic/Cube.h"
#include "Objects/Basic/Spher.h"

// Planet Header
#include "Objects/Planet/Earth.h"
#include "Objects/Planet/Sun.h"
#include "Objects/Planet/Moon.h"

// Robot Header
#include "Objects/Robot/Robot.h"

// Game Box Header
#include "Objects/Basic/GameBox.h"

// Ground Header
#include "Objects/Ground/Ground.h"

// Scene
#include "Scenes/Scene.h"
#include "Manager/GameManager.h"
#include "Manager/SceneManager.h"

#include "Scenes/GameScene.h"

extern HWND hWnd;