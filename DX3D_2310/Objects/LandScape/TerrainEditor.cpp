#include "Framework.h"

TerrainEditer::TerrainEditer() : width(MAX_SIZE), height(MAX_SIZE)
{
	material->SetDiffuseMap(L"Textures/Landscape/Dirt2.png");
	material->SetShader(L"LandScape/TerrainEditer.hlsl");
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

TerrainEditer::~TerrainEditer()
{
	delete mesh;

	delete structuredBuffer;
	delete rayBuffer;
	delete brushBuffer;
}

void TerrainEditer::Update()
{
	//Picking();
	ComputePicking(pickingPos);
	brushBuffer->Get().pickingPos = pickingPos;

	if (KEY->Press(VK_LBUTTON) && !ImGui::GetIO().WantCaptureMouse)
	{
		AdjustHeight();
	}
}

void TerrainEditer::Render()
{
	brushBuffer->SetPS(10);
	SetRender();
	mesh->Draw();
}

void TerrainEditer::GUIRender()
{
	string def = pickingPos;
	ImGui::Text(def.c_str());
	ImGui::DragFloat("Range", &brushBuffer->Get().range);
	ImGui::ColorEdit3("BrushColor", (float*)&brushBuffer->Get().color);
	ImGui::DragFloat("AdjustHeight", &adjustValue);

	SaveHeightMap();
	ImGui::SameLine();
	LoadHeightMap();
}

void TerrainEditer::Picking()
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
// ���� �������� ��ǻƮ ��ŷ �Լ��Դϴ�.
bool TerrainEditer::ComputePicking(Vector3& pos)
{
	// ���콺 ��ġ�κ��� ���� ����
	Ray ray = CAM->ScreenPointToRay(mousePos);

	// ���̿� �ﰢ���� ���� ���θ� ����ó���ϱ� ���� �ʿ��� ������ ����
	rayBuffer->Get().pos = ray.pos;
	rayBuffer->Get().dir = ray.dir;
	rayBuffer->Get().triangleSize = triangleSize;

	// ���� ���۸� Compute Shader�� ����
	rayBuffer->SetCS(0);

	// ����ȭ�� ���۸� Compute Shader���� �б� ���� SRV�� ����
	DC->CSSetShaderResources(0, 1, &structuredBuffer->GetSRV());

	// ����ȭ�� ���۸� Compute Shader���� ���� ���� UAV�� ����
	DC->CSSetUnorderedAccessViews(0, 1, &structuredBuffer->GetUAV(), nullptr);

	// Compute Shader�� ����
	computeShader->Set();

	// ������ �׷� �� ��� (�ø� ó��)
	UINT x = ceil((float)triangleSize / 64.0f);

	// Compute Shader ����
	DC->Dispatch(x, 1, 1);
	//---------------------------------------
	// �� ���Ŀ� outputs�����Ϳ� ������ó������� ����ȴ�.


	// ����� CPU�� ����
	structuredBuffer->Copy(outputs.data(), sizeof(OutputDesc) * triangleSize);

	// �ּ� �Ÿ��� �ش� �ε��� �ʱ�ȭ
	float minDistance = FLT_MAX;
	int minIndex = -1;

	// ����� �м��Ͽ� �ּ� �Ÿ��� ã��
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

	// �ּ� �Ÿ��� ������Ʈ�Ǿ��ٸ� ���� ������ ����ϰ� true ��ȯ
	if (minIndex >= 0)
	{
		pos = ray.pos + ray.dir * minDistance;
		return true;
	}

	// ���� ������ ���� ��� false ��ȯ
	return false;
}

// ���� �������� ��ǻƮ ������ ���� �Լ��Դϴ�.
void TerrainEditer::MakeComputeData()
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

Vector3 TerrainEditer::GetOngravityAcceleration(const Vector3 ObjectPos, const Vector3 correction)
{
    return Vector3();
}

bool TerrainEditer::ChackOnGround(const Vector3 ObjectPos)
{
    return false;
}

pair<Vector3, Vector3> TerrainEditer::FindLowPos(Vector3 e0, Vector3 e1, Vector3 e2, Vector3 e3)
{
    return pair<Vector3, Vector3>();
}

void TerrainEditer::MakeMesh(bool tile, bool flip)
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

void TerrainEditer::MakeNormal(bool Flip)
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

// �귯�ø� ����Ͽ� ������ ���̸� �������� �����ϴ� �Լ��Դϴ�.
void TerrainEditer::AdjustHeight()
{
	// ���� �޽��� �������� �����ɴϴ�.
	vector<VertexType>& vertices = mesh->GetVertices();

	// �� ������ ���� �귯�� ������ ����ϰ� ���̸� �����մϴ�.
	for (VertexType& vertex : vertices)
	{
		// ������ x, z ��ǥ�� �����ϴ� 3D ��ġ ����
		Vector3 pos = Vector3(vertex.pos.x, 0.0f, vertex.pos.z);

		// �귯���� ��ŷ ��ġ�� �������� y ��ǥ�� 0���� ����
		Vector3 pickingPos = brushBuffer->Get().pickingPos;
		pickingPos.y = 0.0f;

		// ������ �귯�� ��ŷ ��ġ ���� �Ÿ� ���
		float distance = MATH->Distance(pos, pickingPos);

		// �Ÿ��� �귯���� ȿ�� ���� ���� �ִ� ��쿡�� ���̸� �����մϴ�.
		if (distance <= brushBuffer->Get().range)
		{
			// ���̸� �����ϴ� �κ�
			vertex.pos.y += adjustValue * DELTA;

			// ���̸� �ִ� ����(MAX_HEIGHT)�� �����մϴ�.
			vertex.pos.y = MATH->Clamp(0.0f, MAX_HEIGHT, vertex.pos.y);
		}
	}

	// ���̰� ������ �Ŀ��� �޽��� ���̸� �� �븻�� ������Ʈ�մϴ�.
	UpdateHeight();
}

void TerrainEditer::AdjustAlpha()
{

}

void TerrainEditer::SaveHeightMap()
{
	// "SaveHeight" ��ư�� ���ȴ��� Ȯ���ϰ�, ���ȴٸ� ���� ���̾�α׸� ���ϴ�.
	if (ImGui::Button("SaveHeight"))
		DIALOG->OpenDialog("SaveHeight", "SaveHeight", ".png", ".");

	// ���� ���̾�αװ� �����ִ� ���
	if (DIALOG->Display("SaveHeight"))
	{
		// "OK" ��ư�� ���ȴ��� Ȯ��
		if (DIALOG->IsOk())
		{
			// ���õ� ���� ��θ� �����ͼ� ������Ʈ ��� ������ ��� ��η� ��ȯ�մϴ�.
			string file = DIALOG->GetFilePathName();
			file = file.substr(projectPath.size() + 1, file.size());

			// �̹����� ũ�⿡ �°� �ȼ� �����͸� ������ �迭�� �������� �Ҵ��մϴ�.
			UINT size = width * height * 4;
			uint8_t* pixels = new uint8_t[size];

			// �޽��� �������� ���� ������ ������ �ȼ� �����ͷ� ��ȯ�մϴ�.
			vector<VertexType>& vertices = mesh->GetVertices();
			FOR(size / 4)
			{
				float y = vertices[i].pos.y;

				// ���� ���� 0���� MAX_HEIGHT������ ������ ����ȭ�Ͽ� 0���� 255 ������ ������ ��ȯ�մϴ�.
				uint8_t height = y / MAX_HEIGHT * 255;

				// �ȼ� �����Ϳ� �����մϴ�. (RGBA ����)
				pixels[i * 4 + 0] = height;
				pixels[i * 4 + 1] = height;
				pixels[i * 4 + 2] = height;
				pixels[i * 4 + 3] = 255;
			}

			// �̹��� ������ ��� ����ü�� �ʱ�ȭ�ϰ� �ȼ� �����͸� �Ҵ�� �޸𸮿� �����մϴ�.
			Image image;
			image.width = width;
			image.height = height;
			image.format = DXGI_FORMAT_R8G8B8A8_UNORM;
			image.rowPitch = width * 4;
			image.slicePitch = size;
			image.pixels = pixels;

			// WIC ���Ϸ� �̹����� �����մϴ�. (PNG ����)
			SaveToWICFile(image, WIC_FLAGS_FORCE_RGB,
				GetWICCodec(WIC_CODEC_PNG), ToWString(file).c_str());

			// �������� �Ҵ�� �ȼ� �������� �޸𸮸� �����մϴ�.
			delete[] pixels;
		}

		// ���� ���̾�α׸� �ݽ��ϴ�.
		DIALOG->Close();
	}
}

void TerrainEditer::LoadHeightMap()
{
	// "LoadHeight" ��ư�� ���ȴ��� Ȯ���ϰ�, ���ȴٸ� ���� ���̾�α׸� ���ϴ�.
	if (ImGui::Button("LoadHeight"))
		DIALOG->OpenDialog("LoadHeight", "LoadHeight", ".png", ".");

	// ���� ���̾�αװ� �����ִ� ���
	if (DIALOG->Display("LoadHeight"))
	{
		// "OK" ��ư�� ���ȴ��� Ȯ��
		if (DIALOG->IsOk())
		{
			// ���õ� ���� ��θ� �����ͼ� ������Ʈ ��� ������ ��� ��η� ��ȯ�մϴ�.
			string file = DIALOG->GetFilePathName();
			file = file.substr(projectPath.size() + 1, file.size());

			// �ؽ�ó�� ���̸��� �ҷ��ɴϴ�.
			heightMap = Texture::Add(ToWString(file));

			// �޽ø� ���������Ͽ� ���ο� ���̸ʿ� �°� �����մϴ�.
			Resize();
		}

		// ���� ���̾�α׸� �ݽ��ϴ�.
		DIALOG->Close();
	}
}

void TerrainEditer::SaveAlphaMap()
{
}

void TerrainEditer::LoadAlphaMap()
{
}

void TerrainEditer::UpdateHeight()
{
	vector<VertexType>& vertices = mesh->GetVertices();
	for (VertexType& vertex : vertices)
		vertex.normal = {};

	MakeNormal();
	MakeComputeData();
	mesh->UpdateVertices();
	structuredBuffer->UpdateInput(inputs.data());
}

void TerrainEditer::Resize()
{
	MakeMesh();
	MakeNormal();
	MakeComputeData();

	mesh->UpdateVertices();
	mesh->UpdateIndices();

	structuredBuffer->UpdateInput(inputs.data());
}
