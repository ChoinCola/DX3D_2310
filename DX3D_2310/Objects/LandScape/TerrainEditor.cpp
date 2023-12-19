#include "Framework.h"

TerrainEditor::TerrainEditor() : width(MAX_SIZE), height(MAX_SIZE)
{
	material->SetDiffuseMap(L"Textures/Landscape/Dirt2.png");
	material->SetShader(L"LandScape/TerrainEditer.hlsl");

	secondMap = Texture::Add(L"Textures/Landscape/Stones.png");
	thirdMap = Texture::Add(L"Textures/Landscape/Bricks.png");

	mesh = new Mesh<VertexType>;
	MakeMesh();
	MakeNormal();
	MakeComputeData();
	mesh->CreateMesh();

	computeShader = Shader::AddCS(L"Compute/ComputePicking.hlsl");

	structuredBuffer = new StructuredBuffer(
		inputs.data(), sizeof(InputDesc), triangleSize,
		sizeof(OutputDesc), triangleSize);
	rayBuffer = new RayBuffer();
	brushBuffer = new BrushBuffer();

	char path[128];
	GetCurrentDirectoryA(128, path);
	projectPath = path;
}

TerrainEditor::~TerrainEditor()
{
	delete mesh;

	delete structuredBuffer;
	delete rayBuffer;
	delete brushBuffer;
}

void TerrainEditor::Update()
{
	//Picking();
	ComputePicking(pickingPos);
	brushBuffer->Get().pickingPos = pickingPos;

	if (KEY->Press(VK_LBUTTON) && !ImGui::GetIO().WantCaptureMouse)
	{
		switch (editType)
		{
		case TerrainEditor::HEIGHT:
			AdjustHeight();
			break;
		case TerrainEditor::ALPHA:
			AdjustAlpha();
			break;
		default:
			break;
		}
	}
}

void TerrainEditor::Render()
{
	brushBuffer->SetPS(10);
	secondMap->PSSet(11);
	thirdMap->PSSet(12);

	SetRender();
	mesh->Draw();
}

void TerrainEditor::GUIRender()
{
	string def = pickingPos;
	ImGui::Text(def.c_str());

	const char* editList[] = { "Height", "Alpha" };
	ImGui::Combo("EditType", (int*)&editType, editList, 2);


	ImGui::DragFloat("BrushRange", &brushBuffer->Get().range);
	ImGui::ColorEdit3("BrushColor", (float*)&brushBuffer->Get().color);
	ImGui::DragFloat("AdjustHeight", &adjustValue);
	ImGui::DragInt("SecondMap", &selectMap);

	SaveHeightMap();
	ImGui::SameLine();
	LoadHeightMap();

	SaveAlphaMap();
	ImGui::SameLine();
	LoadAlphaMap();
}

void TerrainEditor::Picking()
{
	Ray ray = CAM->ScreenPointToRay(mousePos);

	for (UINT z = 0; z < height - 1; z++)
	{
		for (UINT x = 0; x < width - 1; x++)
		{
			UINT index[4];
			index[0] = width * z + x;
			index[1] = width * z + x + 1;
			index[2] = width * (z + 1) + x;
			index[3] = width * (z + 1) + x + 1;

			vector<VertexType>& vertices = mesh->GetVertices();

			Vector3 p[4];
			FOR(4)
				p[i] = vertices[index[i]].pos;

			float distance = 0.0f;

			if (Intersects(ray.pos, ray.dir, p[0], p[1], p[2], distance))
			{
				pickingPos = ray.pos + ray.dir * distance;
				return;
			}
			if (Intersects(ray.pos, ray.dir, p[3], p[1], p[2], distance))
			{
				pickingPos = ray.pos + ray.dir * distance;
				return;
			}
		}
	}
}
// 지형 편집기의 컴퓨트 피킹 함수입니다.
bool TerrainEditor::ComputePicking(Vector3& pos)
{
	// 마우스 위치로부터 레이 생성
	Ray ray = CAM->ScreenPointToRay(mousePos);

	// 레이와 삼각형의 교차 여부를 병렬처리하기 위해 필요한 데이터 설정
	rayBuffer->Get().pos = ray.pos;
	rayBuffer->Get().dir = ray.dir;
	rayBuffer->Get().triangleSize = triangleSize;

	// 레이 버퍼를 Compute Shader에 전달
	rayBuffer->SetCS(0);

	// 구조화된 버퍼를 Compute Shader에서 읽기 위한 SRV로 설정
	DC->CSSetShaderResources(0, 1, &structuredBuffer->GetSRV());

	// 구조화된 버퍼를 Compute Shader에서 쓰기 위한 UAV로 설정
	DC->CSSetUnorderedAccessViews(0, 1, &structuredBuffer->GetUAV(), nullptr);

	// Compute Shader를 설정
	computeShader->Set();

	// 쓰레드 그룹 수 계산 (올림 처리)
	UINT x = ceil((float)triangleSize / 64.0f);

	// Compute Shader 실행
	DC->Dispatch(x, 1, 1);
	//---------------------------------------
	// 이 이후에 outputs데이터에 데이터처리결과가 저장된다.


	// 결과를 CPU로 복사
	structuredBuffer->Copy(outputs.data(), sizeof(OutputDesc) * triangleSize);

	// 최소 거리와 해당 인덱스 초기화
	float minDistance = FLT_MAX;
	int minIndex = -1;

	// 결과를 분석하여 최소 거리를 찾음
	UINT index = 0;
	for (OutputDesc output : outputs)
	{
		if (output.picked)
		{
			if (minDistance > output.distance)
			{
				minDistance = output.distance;
				minIndex = index;
			}
		}
		index++;
	}

	// 최소 거리가 업데이트되었다면 교차 지점을 계산하고 true 반환
	if (minIndex >= 0)
	{
		pos = ray.pos + ray.dir * minDistance;
		return true;
	}

	// 교차 지점이 없을 경우 false 반환
	return false;
}

// 지형 편집기의 컴퓨트 데이터 생성 함수입니다.
void TerrainEditor::MakeComputeData()
{
	vector<VertexType> vertices = mesh->GetVertices();
	vector<UINT> indices = mesh->GetIndices();

	triangleSize = indices.size() / 3;

	inputs.resize(triangleSize);
	outputs.resize(triangleSize);

	for (UINT i = 0; i < triangleSize; i++)
	{
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];

		inputs[i].v0 = vertices[index0].pos;
		inputs[i].v1 = vertices[index1].pos;
		inputs[i].v2 = vertices[index2].pos;
	}
}

Vector3 TerrainEditor::GetOngravityAcceleration(const Vector3 ObjectPos, const Vector3 correction)
{
    return Vector3();
}

bool TerrainEditor::ChackOnGround(const Vector3 ObjectPos)
{
    return false;
}

pair<Vector3, Vector3> TerrainEditor::FindLowPos(Vector3 e0, Vector3 e1, Vector3 e2, Vector3 e3)
{
    return pair<Vector3, Vector3>();
}

void TerrainEditor::MakeMesh(bool tile, bool flip)
{
	vector<Float4> pixels(width * height, Float4(0, 0, 0, 0));
	if (heightMap)
	{
		width = heightMap->GetSize().x;
		height = heightMap->GetSize().y;

		heightMap->ReadPixels(pixels);
	}
	//Vertices
	vector<VertexType>& vertices = mesh->GetVertices();
	vertices.clear();

	vertices.reserve(width * height);
	for (UINT z = 0; z < height; z++)
	{
		for (UINT x = 0; x < width; x++)
		{
			VertexType vertex;
			vertex.pos = { (float)x, 0.0f, (float)z };
			vertex.uv.x = x / (float)(width - 1);
			vertex.uv.y = z / (float)(height - 1);

			UINT index = width * z + x;
			vertex.pos.y = pixels[index].x * 20.0f;

			vertices.push_back(vertex);
		}
	}

	//Indices
	vector<UINT>& indices = mesh->GetIndices();
	indices.clear();

	indices.reserve((width - 1) * (height - 1) * 6);

	for (UINT z = 0; z < height - 1; z++)
	{
		for (UINT x = 0; x < width - 1; x++)
		{
			indices.push_back(width * z + x);//0
			indices.push_back(width * (z + 1) + x);//1
			indices.push_back(width * z + x + 1);//2

			indices.push_back(width * z + x + 1);//2
			indices.push_back(width * (z + 1) + x);//1
			indices.push_back(width * (z + 1) + x + 1);//3
		}
	}
}

void TerrainEditor::MakeNormal(bool Flip)
{
	vector<VertexType>& vertices = mesh->GetVertices();
	vector<UINT>& indices = mesh->GetIndices();

	FOR(indices.size() / 3)
	{
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];

		Vector3 v0 = vertices[index0].pos;
		Vector3 v1 = vertices[index1].pos;
		Vector3 v2 = vertices[index2].pos;

		Vector3 e0 = v1 - v0;
		Vector3 e1 = v2 - v0;

		Vector3 noraml = Vector3::Cross(e0, e1).GetNormalized();

		if (Flip) {
			vertices[index0].normal -= noraml;
			vertices[index1].normal -= noraml;
			vertices[index2].normal -= noraml;
		}
		else
		{
			vertices[index0].normal += noraml;
			vertices[index1].normal += noraml;
			vertices[index2].normal += noraml;
		}
	}
}

// 브러시를 사용하여 지형의 높이를 동적으로 조절하는 함수입니다.
void TerrainEditor::AdjustHeight()
{
	// 지형 메시의 정점들을 가져옵니다.
	vector<VertexType>& vertices = mesh->GetVertices();

	// 각 정점에 대해 브러시 영향을 계산하고 높이를 조절합니다.
	for (VertexType& vertex : vertices)
	{
		// 정점의 x, z 좌표를 포함하는 3D 위치 생성
		Vector3 pos = Vector3(vertex.pos.x, 0.0f, vertex.pos.z);

		// 브러시의 피킹 위치를 가져오고 y 좌표를 0으로 설정
		Vector3 pickingPos = brushBuffer->Get().pickingPos;
		pickingPos.y = 0.0f;

		// 정점과 브러시 피킹 위치 간의 거리 계산
		float distance = MATH->Distance(pos, pickingPos);

		// 거리가 브러시의 효과 범위 내에 있는 경우에만 높이를 조절합니다.
		if (distance <= brushBuffer->Get().range)
		{
			// 높이를 조절하는 부분
			vertex.pos.y += adjustValue * DELTA;

			// 높이를 최대 높이(MAX_HEIGHT)로 제한합니다.
			vertex.pos.y = MATH->Clamp(0.0f, MAX_HEIGHT, vertex.pos.y);
		}
	}

	// 높이가 조절된 후에는 메시의 높이맵 및 노말을 업데이트합니다.
	UpdateHeight();
}

void TerrainEditor::AdjustAlpha()
{
	// 지형 메시의 정점들을 가져옵니다.
	vector<VertexType>& vertices = mesh->GetVertices();

	// 각 정점에 대해 브러시 영향을 계산하고 높이를 조절합니다.
	for (VertexType& vertex : vertices)
	{
		// 정점의 x, z 좌표를 포함하는 3D 위치 생성
		Vector3 pos = Vector3(vertex.pos.x, 0.0f, vertex.pos.z);

		// 브러시의 피킹 위치를 가져오고 y 좌표를 0으로 설정
		Vector3 pickingPos = brushBuffer->Get().pickingPos;
		pickingPos.y = 0.0f;

		// 정점과 브러시 피킹 위치 간의 거리 계산
		float distance = MATH->Distance(pos, pickingPos);

		// 거리가 브러시의 효과 범위 내에 있는 경우에만 높이를 조절합니다.
		if (distance <= brushBuffer->Get().range)
		{
			vertex.alpha[selectMap] += adjustValue * DELTA;
			vertex.alpha[selectMap] = MATH->Clamp(0.0f, 1.0f, vertex.alpha[selectMap]);
		}
	}

	// 높이가 조절된 후에는 메시의 높이맵 및 노말을 업데이트합니다.
	UpdateHeight();
}

void TerrainEditor::SaveHeightMap()
{
	// "SaveHeight" 버튼이 눌렸는지 확인하고, 눌렸다면 파일 다이얼로그를 엽니다.
	if (ImGui::Button("SaveHeight"))
		DIALOG->OpenDialog("SaveHeight", "SaveHeight", ".png", ".");

	// 파일 다이얼로그가 열려있는 경우
	if (DIALOG->Display("SaveHeight"))
	{
		// "OK" 버튼이 눌렸는지 확인
		if (DIALOG->IsOk())
		{
			// 선택된 파일 경로를 가져와서 프로젝트 경로 이후의 상대 경로로 변환합니다.
			string file = DIALOG->GetFilePathName();
			file = file.substr(projectPath.size() + 1, file.size());

			// 이미지의 크기에 맞게 픽셀 데이터를 저장할 배열을 동적으로 할당합니다.
			UINT size = width * height * 4;
			uint8_t* pixels = new uint8_t[size];

			// 메시의 정점에서 높이 정보를 가져와 픽셀 데이터로 변환합니다.
			vector<VertexType>& vertices = mesh->GetVertices();
			FOR(size / 4)
			{
				float y = vertices[i].pos.y;

				// 높이 값을 0에서 MAX_HEIGHT까지의 범위로 정규화하여 0에서 255 사이의 값으로 변환합니다.
				uint8_t height = y / MAX_HEIGHT * 255;

				// 픽셀 데이터에 저장합니다. (RGBA 형식)
				pixels[i * 4 + 0] = height;
				pixels[i * 4 + 1] = height;
				pixels[i * 4 + 2] = height;
				pixels[i * 4 + 3] = 255;
			}

			// 이미지 정보를 담는 구조체를 초기화하고 픽셀 데이터를 할당된 메모리에 복사합니다.
			Image image;
			image.width = width;
			image.height = height;
			image.format = DXGI_FORMAT_R8G8B8A8_UNORM;
			image.rowPitch = width * 4;
			image.slicePitch = size;
			image.pixels = pixels;

			// WIC 파일로 이미지를 저장합니다. (PNG 형식)
			SaveToWICFile(image, WIC_FLAGS_FORCE_RGB,
				GetWICCodec(WIC_CODEC_PNG), ToWString(file).c_str());

			// 동적으로 할당된 픽셀 데이터의 메모리를 해제합니다.
			delete[] pixels;
		}

		// 파일 다이얼로그를 닫습니다.
		DIALOG->Close();
	}
}

void TerrainEditor::LoadHeightMap()
{
	// "LoadHeight" 버튼이 눌렸는지 확인하고, 눌렸다면 파일 다이얼로그를 엽니다.
	if (ImGui::Button("LoadHeight"))
		DIALOG->OpenDialog("LoadHeight", "LoadHeight", ".png", ".");

	// 파일 다이얼로그가 열려있는 경우
	if (DIALOG->Display("LoadHeight"))
	{
		// "OK" 버튼이 눌렸는지 확인
		if (DIALOG->IsOk())
		{
			// 선택된 파일 경로를 가져와서 프로젝트 경로 이후의 상대 경로로 변환합니다.
			string file = DIALOG->GetFilePathName();
			file = file.substr(projectPath.size() + 1, file.size());

			// 텍스처로 높이맵을 불러옵니다.
			heightMap = Texture::Add(ToWString(file));

			// 메시를 리사이즈하여 새로운 높이맵에 맞게 조정합니다.
			Resize();
		}

		// 파일 다이얼로그를 닫습니다.
		DIALOG->Close();
	}
}

void TerrainEditor::SaveAlphaMap()
{
	if (ImGui::Button("SaveAlpha"))
		DIALOG->OpenDialog("SaveAlpha", "SaveAlpha", ".png", ".");

	if (DIALOG->Display("SaveAlpha"))
	{
		if (DIALOG->IsOk())
		{
			string file = DIALOG->GetFilePathName();

			file = file.substr(projectPath.size() + 1, file.size());

			UINT size = width * height * 4;
			uint8_t* pixels = new uint8_t[size];

			vector<VertexType>& vertices = mesh->GetVertices();

			FOR(size / 4)
			{
				pixels[i * 4 + 0] = vertices[i].alpha[0] * 255;
				pixels[i * 4 + 1] = vertices[i].alpha[1] * 255;
				pixels[i * 4 + 2] = vertices[i].alpha[2] * 255;
				pixels[i * 4 + 3] = 255;
			}

			Image image;
			image.width = width;
			image.height = height;
			image.format = DXGI_FORMAT_R8G8B8A8_UNORM;
			image.rowPitch = width * 4;
			image.slicePitch = size;
			image.pixels = pixels;

			SaveToWICFile(image, WIC_FLAGS_FORCE_RGB,
				GetWICCodec(WIC_CODEC_PNG), ToWString(file).c_str());

			delete[] pixels;
		}

		DIALOG->Close();
	}
}

void TerrainEditor::LoadAlphaMap()
{
	if (ImGui::Button("LoadAlpha"))
		DIALOG->OpenDialog("LoadAlpha", "LoadAlpha", ".png", ".");

	if (DIALOG->Display("LoadAlpha"))
	{
		if (DIALOG->IsOk())
		{
			string file = DIALOG->GetFilePathName();

			file = file.substr(projectPath.size() + 1, file.size());

			Texture* alphaMap = Texture::Add(ToWString(file));

			vector<Float4> pixels;
			alphaMap->ReadPixels(pixels);

			vector<VertexType>& vertices = mesh->GetVertices();

			FOR(vertices.size())
			{
				vertices[i].alpha[0] = pixels[i].z;
				vertices[i].alpha[1] = pixels[i].y;
				vertices[i].alpha[2] = pixels[i].x;
				vertices[i].alpha[3] = pixels[i].w;
			}

			mesh->UpdateVertices();
		}

		DIALOG->Close();
	}
}

void TerrainEditor::UpdateHeight()
{
	vector<VertexType>& vertices = mesh->GetVertices();
	for (VertexType& vertex : vertices)
		vertex.normal = {};

	MakeNormal();
	MakeComputeData();
	mesh->UpdateVertices();
	structuredBuffer->UpdateInput(inputs.data());
}

void TerrainEditor::Resize()
{
	MakeMesh();
	MakeNormal();
	MakeComputeData();

	mesh->UpdateVertices();
	mesh->UpdateIndices();

	structuredBuffer->UpdateInput(inputs.data());
}
