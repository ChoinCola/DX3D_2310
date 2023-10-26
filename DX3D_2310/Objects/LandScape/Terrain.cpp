#include "Framework.h"

Terrain::Terrain()
{
	SetLocalPosition({ 0, 0, 0 });
	tag = "Terrain";
	material->SetShader(L"Light/SpecularLight.hlsl");
	material->SetDiffuseMap(L"Textures/Colors/White.png");

	heightMap = Texture::Add(L"Textures/HeightMaps/HeightMap.png");

	mesh = new Mesh<VertexUVNormal>();
	normalline = new Mesh<VertexColor>();
	RSset = new RasterizerState();

	MakeMesh();
	MakeNormal();
	mesh->CreateMesh();
	normalline->CreateMesh();
	RSset->SetState();
}

Terrain::~Terrain()
{
	delete mesh;
}

void Terrain::Render()
{
	SetRender();
	if (IsPrintNormalLine)
		normalline->Draw(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	if (IsMapLine)
	{
		RSset->CullMode(D3D11_CULL_NONE);
		RSset->FillMode(D3D11_FILL_WIREFRAME);
		RSset->SetState();
		ChackOne = true;
	}

	if (!IsMapLine) {
		RSset->CullMode(D3D11_CULL_BACK);
		RSset->FillMode(D3D11_FILL_SOLID);
		RSset->SetState();
		ChackOne = false;
	}

	mesh->Draw();
}

void Terrain::GUIRender()
{
	ImGui::Checkbox("Terrain_Print_NormalLine", &IsPrintNormalLine);
	ImGui::Checkbox("Terrain_Onley_Line", &IsMapLine);
	ImGui::Text("x: %d, z: %d, y:%d", x, z, y);
	ImGui::Text("normalize : x: %d, y: %d, z: %d", v.x, v.y, v.z);
	ImGui::Text("D : %d", D);

	__super::GUIRender();
}

Vector3 Terrain::GetOnGrondPosition(Vector3 ObjectPos)
{
	// ���� �� �ΰ�� �ڱ��ڽ� ��ȯ
	// ��ǥ�� �׻� ����ġ�� �����ġ�� ����Ѵ�.
	Vector3 LocalMaxPos =  Vector3(width, 0, height) + GetLocalPosition();
	Vector3 LocalLowPos = GetLocalPosition();

	if (ObjectPos.x > LocalMaxPos.x || ObjectPos.z > LocalMaxPos.z ||
		ObjectPos.x < LocalLowPos.x || ObjectPos.z < LocalLowPos.z)
		return ObjectPos;

	// ���� �� �ϰ��,
	// ���� Object��ġ�� XY��ǥ�� ��ȯ�Ͽ� ��� �簢�� �ȿ� �����ִ��� �Ǻ���.
	// �⺻ ����Ʈ ĭ ������� xy�� 1��
	// ���� xy��ǥ�� int�� ��ȯ�ϸ� ���� Pos�� ĭ ���ΰ� ��Ÿ��.
	vector<VertexType>& vertices = mesh->GetVertices();
	vector<UINT>& indices = mesh->GetIndices();

	x = ObjectPos.x;
	z = ObjectPos.z;

	// indicees�� �����ؼ� �����͸� ��������, 3�� 0�� �� �����vertices�� ã�ƾ���.
	// �簢���� �ƴ� �ﰢ������ vertices�� �׾��������� �ﰢ�� �������� ���̰��� ���ؾ��ϱ� ����.

	Vector3 e0 = vertices[width * z + x].pos;	// 0
	Vector3 e1 = vertices[width * (z + 1) + x].pos;	// 1
	Vector3 e2 = vertices[width * z + x + 1].pos;	// 2
	Vector3 e3 = vertices[width * (z + 1) + x + 1].pos;	// 3

	// ��Į���� ������. e0�� �� �������, e0���� ����
	if (ObjectPos.x <  ObjectPos.z)
	{
		// e0e1, e0e2 ����.
		v = Vector3::ComputeNormal(e0, e1, e2);
		//��������,
		// x + y + z + D = 0;
		// D ���ϱ�.
		// D = -Ax1 - By1 - Cz1; �������� �������� �Ÿ�
		// D = -v.x * e0.x + -v.y * e0.y + -v.z * e0.z
		D = -v.x * e0.x - v.y * e0.y - v.z * e0.z;
		// ���� ������. y = - x - z - D
		y = -(v.x * ObjectPos.x + v.z * ObjectPos.z + D) / v.y;

		return Vector3(ObjectPos.x, y, ObjectPos.z);

	}
	// e3�� �� �����ų� �������, e3�� ����.
	else
	{
		// e0e1, e0e2 ����.
		v = Vector3::ComputeNormal(e3, e1, e2);
		//��������,
		// x + y + z + D = 0;
		// D ���ϱ�.
		// D = -Ax1 - By1 - Cz1;
		// D = -v.x * e0.x + -v.y * e0.y + -v.z * e0.z
		D = -v.x * e3.x - v.y * e3.y - v.z * e3.z;
		// ���� ������. y = - x - z - D
		y = -(v.x * ObjectPos.x + v.z * ObjectPos.z + D) / v.y;

		return Vector3(ObjectPos.x, y, ObjectPos.z);
	}

}

void Terrain::MakeMesh()
{
	width = heightMap->GetSize().x;
	height = heightMap->GetSize().y;

	vector<Float4> pixels;
	heightMap->ReadPixels(pixels);

	vector<VertexType>& vertices = mesh->GetVertices();

	vertices.reserve(width * height);
	for (UINT z = 0; z < height; z++) {
		for (UINT x = 0; x < width; x++) {
			VertexType vertex;
			vertex.pos = { (float)x, 0.0f, (float)z };
			vertex.uv.x = x / (float)(width - 1);
			vertex.uv.y = z / (float)(height - 1);

			UINT index = width * z + x;
			vertex.pos.y = pixels[index].x * 20.0f;

			vertices.push_back(vertex);
		}
	}

	// Indices
	vector<UINT>& indices = mesh->GetIndices();

	indices.reserve((width - 1) * (height - 1) * 6);

	for (UINT z = 0; z < height - 1; z++) {
		for (UINT x = 0; x < width - 1; x++) {
			indices.push_back(width * z + x);	// 0
			indices.push_back(width * (z + 1) + x);	// 1
			indices.push_back(width * z + x + 1);	// 2

			indices.push_back(width * z + x + 1);	// 2
			indices.push_back(width * (z + 1) + x);	// 1
			indices.push_back(width * (z + 1) + x + 1);	// 3
		}
	}

}

void Terrain::MakeNormal()
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

		vertices[index0].normal += noraml;
		vertices[index1].normal += noraml;
		vertices[index2].normal += noraml;
	}
}
