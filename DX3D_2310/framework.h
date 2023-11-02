#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN

#define WIN_START_X 100
#define WIN_START_Y 100

#define WIN_WIDTH 1280
#define WIN_HEIGHT 720

#define CENTER_X WIN_WIDTH * 0.5f
#define CENTER_Y WIN_HEIGHT * 0.5f

#define MAX_LIGHT 100

#define DEVICE		Device::Get()->GetDevice()
#define DC			Device::Get()->GetDeviceContext()
#define KEY			Keyboard::Get()
#define DELTA		Timer::Get()->GetElapsedTime()
#define DIALOG		ImGuiFileDialog::Instance()

#define FOR(n) for(UINT i = 0; i < n; i++)

#define GRAVITY(g) { g = 0.98 * g }

#define SAFE_DELETE(p)		 { if(p) { delete(p);	   (p) = nullptr; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[](p);	   (p) = nullptr; } }
#define SAFE_RELEASE(p)		 { if(p) { (p)->Release(); (p) = nullptr; } }

#define CHECK(p) assert(SUCCEEDED(p))

#define CAM Environment::Get()->GetMainCamera()

#include <windows.h>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <list>
#include <algorithm>
#include <random>
#include <assert.h>

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

// Assimp
#include <Assimp/Importer.hpp>
#include <Assimp/scene.h>
#include <Assimp/postprocess.h>
#pragma comment(lib, "assimp-vc143-mtd.lib")


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
#include "Framework/Utilities/Utility.h"
#include "Framework/Utilities/Timer.h"
#include "Framework/Utilities/Mouse.h"
#include "Framework/Utilities/BinaryReader.h"
#include "Framework/Utilities/BinaryWriter.h"
#include "Framework/Utilities/tinyxml2.h"

using namespace Utility;

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

// State Header
#include "Framework/State/RasterizerState.h"
#include "Framework/State/SamplerState.h"
#include "Framework/State/BlendState.h"

// Math Header
#include "Framework/Math/Vector3.h"
#include "Framework/Math/Transform.h"

#include "Framework/Collider/Collider.h"
#include "Framework/Collider/CapsuleCollider.h"
#include "Framework/Collider/BoxCollider.h"
#include "Framework/Collider/CapsuleCollider.h"
#include "Framework/Collider/SphereCollider .h"

// View Header
#include "Framework/Environment/Camera.h"
#include "Framework/Environment/Minimap.h"
#include "Framework/Environment/Environment.h"

// Model Header
#include "Framework/Model/ModelExporter.h"

// Obejct Header
#include "Objects/Basic/GameObject.h"
#include "Objects/Basic/Quad.h"
#include "Objects/Basic/Cube.h"
#include "Objects/Basic/Sphere.h"
#include "Objects/Basic/Icosahedron.h"

#include "Objects/ColliderBall/Ball.h"

// Robot Header
#include "Objects/Robot/Robot.h"

// Game Box Header
#include "Objects/Basic/GameBox.h"

// Ground Header
#include "Objects/Ground/Ground.h"

// Terrain Header
#include "Objects/LandScape/Terrain.h"

// Shooting Header
#include "Objects/Shooting/Item.h"
#include "Objects/Shooting/Coin.h"
#include "Objects/Shooting/Bullet.h"
#include "Objects/Shooting/ShootingPlayer.h"

// Scene
#include "Scenes/Scene.h"
#include "Scenes/TerrainScene.h"

#include "Manager/GameManager.h"
#include "Manager/SceneManager.h"

// Model Export Scene
#include "Scenes/ModelExportScene.h"

#include "Scenes/GameScene.h"
#include "Scenes/SpherScene.h"

#include "Scenes/CollisionScene.h"

#include "Scenes/FpsScene.h"

#include "Scenes/ShootingScene.h"

#include "Scenes/Pacman.h"
#include "Scenes/LightScene.h"
extern HWND hWnd;
extern Vector3 mousePos;