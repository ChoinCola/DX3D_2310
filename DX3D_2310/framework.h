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
#define MAX_BONE 256
#define MAX_FRAME 256
#define MAX_INSTANCE 500

#define DEVICE		Device::Get()->GetDevice()
#define DC			Device::Get()->GetDeviceContext()
#define KEY			Keyboard::Get()
#define DELTA		Timer::Get()->GetElapsedTime()
#define DIALOG		ImGuiFileDialog::Instance()
#define MATH GameMath::Get()

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
#include <fstream>
#include <functional>

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

// Font
#include <d2d1_2.h>
#include <dwrite.h>
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

using namespace std;
using namespace DirectX;

typedef XMFLOAT4 Float4;
typedef XMFLOAT3 Float3;
typedef XMFLOAT2 Float2;
typedef XMMATRIX Matrix;
typedef XMVECTOR Vector4;	// 다양한 사용방식이 보장된 vector
typedef XMFLOAT4X4 Float4x4;
typedef function<void()> Event;
typedef function<void(void*)> ParamEvent;
typedef function<void(void*, void*)> ParamEvent2;
typedef function<void(int)> IntParamEvent;


// Framework Header
#include "Framework/Utilities/Singleton.h"
#include "Framework/Utilities/Keyboard.h"
#include "Framework/Utilities/Utility.h"
#include "Framework/Utilities/Timer.h"
#include "Framework/Utilities/Mouse.h"
#include "Framework/Utilities/BinaryReader.h"
#include "Framework/Utilities/BinaryWriter.h"
#include "Framework/Utilities/tinyxml2.h"
#include "Framework/Utilities/Font.h"
#include "Framework/Utilities/Observer.h"

// Readcsv
#include "Framework/Utilities/rapidcsv.h"

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
#include "Framework/Buffer/DepthBuffer.h"

// Render Header
#include "Framework/Render/Texture.h"
#include "Framework/Render/Material.h"
#include "Framework/Render/Mesh.h"

// State Header
#include "Framework/State/RasterizerState.h"
#include "Framework/State/SamplerState.h"
#include "Framework/State/BlendState.h"
#include "Framework/State/DepthStencilState.h"

// Math Header
#include "Framework/Math/Vector3.h"
#include "Framework/Math/Transform.h"
#include "Framework/Math/GameMath.h"

// Collider Header
#include "Framework/Collider/Collider.h"
#include "Framework/Collider/BoxCollider.h"
#include "Framework/Collider/CapsuleCollider.h"
#include "Framework/Collider/SphereCollider.h"
// View Header
#include "Framework/Environment/Camera.h"
#include "Framework/Environment/Minimap.h"
#include "Framework/Environment/Environment.h"

// Model Header
#include "Framework/Model/ModelData.h"
#include "Framework/Model/ModelExporter.h"
#include "Framework/Model/ModelMesh.h"
#include "Framework/Model/Model.h"
#include "Framework/Model/ModelInstancing.h"
#include "Framework/Model/ModelClip.h"
#include "Framework/Model/ModelAnimator.h"
#include "Framework/Model/ModelAnimatorInstancing.h"

// Obejct Header
#include "Objects/Basic/GameObject.h"
#include "Objects/Basic/Quad.h"
#include "Objects/Basic/Cube.h"
#include "Objects/Basic/Sphere.h"
#include "Objects/Basic/Icosahedron.h"
#include "Objects/Basic/UI/Button.h"
#include "Objects/Basic/UI/Panel.h"

#include "Objects/ColliderBall/Ball.h"

#include "Framework/Environment/UIRenderMaster.h"

// Robot Header
#include "Objects/Robot/Robot.h"

// Game Box Header
#include "Objects/Basic/GameBox.h"

// Ground Header
#include "Objects/Ground/Ground.h"

// Terrain Header
#include "Objects/LandScape/Terrain.h"
#include "Objects/LandScape/SkyBox.h"

// Shooting Header
//#include "Objects/Shooting/Item.h"
//#include "Objects/Shooting/Coin.h"
//#include "Objects/Shooting/Bullet.h"
//#include "Objects/Shooting/ShootingPlayer.h"

// RPG Header
#include "Objects/MincraftObject/UIManager.h"
#include "Objects/MincraftObject/BlockDataManager.h"
#include "Objects/MincraftObject/Block.h"
#include "Objects/MincraftObject/BlockManager.h"
#include "Objects/MincraftObject/InvenBlock.h"
#include "Objects/MincraftObject/InventoryUI.h"
#include "Objects/MincraftObject/shopUI.h"
#include "Objects/MincraftObject/MineCraftUI.h"
#include "Objects/MincraftObject/Steve.h"
#include "Objects/MincraftObject/BlockIcon.h"
#include "Objects/MincraftObject/Seller.h"
#include "Objects/Raid/Sword.h"
#include "Objects/Raid/UI/HPbar.h"
#include "Objects/Raid/Traveler.h"
#include "Objects/Raid/Monster.h"


// TopViewGame Header
#include "Objects/TopViewGame/_TopViewGamestruct.h"
#include "Objects/TopViewGame/Obejct/Grass.h"
#include "Objects/TopViewGame/Obejct/Trees.h"
#include "Objects/TopViewGame/Monster/TopMonster.h"

#include "Objects/TopViewGame/Master/MonsterMaster.h"
#include "Objects/TopViewGame/Master/ObejctMaster.h"

#include "Objects/TopViewGame/Map.h"
#include "Objects/TopViewGame/TopPlayer.h"


// Scene
#include "Scenes/Scene.h"
#include "Scenes/TerrainScene.h"

#include "Manager/GameManager.h"
#include "Manager/SceneManager.h"

#include "Scenes/AnimationScene.h"
#include "Scenes/GridScene.h"
#include "Scenes/TerrainScene.h"
#include "Scenes/CollisionScene.h"
#include "Scenes/LightScene.h"
#include "Scenes/ModelExportScene.h"
#include "Scenes/ModelRenderScene.h"
#include "Scenes/BoxPlayDemo.h"
#include "Scenes/MineCraftScene.h"
#include "Scenes/RaidScene.h"
#include "Scenes/LightScene.h"
#include "Scenes/InstancingScene.h"
#include "Scenes/TopViewGame.h"
#include "Scenes/ModelInstansingScene.h"

extern HWND hWnd;
extern Vector3 mousePos;