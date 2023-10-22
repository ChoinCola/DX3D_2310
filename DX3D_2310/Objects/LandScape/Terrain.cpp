#include "Framework.h"

Terrain::Terrain()
{
	material->SetDiffuseMap(L"Textures/Colors/White.png");
	material->SetShader(L"Light/DiffuseLight.hlsl");
	RSset = new RasterizerState();
	heightMap = Texture::Add(L"Textures/HeightMaps/HeightMap.png");

	mesh = new Mesh<VertexUVNormal>();
	normalline = new Mesh<VertexColor>();

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

}

void Terrain::MakeMesh()
{
	width = heightMap->GetSize().x;
	height = heightMap->GetSize().y;

	vector<Float4> pixels;
	heightMap->ReadPixels(pixels);

	vector<VertexUVNormal>& vertices = mesh->GetVertices();

	vertices.reserve(width * height);
	for (UINT z = 0; z < height; z++) {
		for (UINT x = 0; x < width; x++) {
			VertexUVNormal vertex;
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
	// �ֺ� Verteices�� ����� indices�� Ž���Ѵ�.
	// ���� ������ ����� ���� �� �ﰢ���� �𼭸����� ���� �ΰ��� ���Ѵ�.
	// xyz�� ������,
	// X = y-x;
	// Y = z-x;
	// 
	// �� �ΰ��� ����.
	// �̶� ���� X = (X_1, X_2, X_3)
	// Y = (Y_1, Y_2, Y_3)
	// �̶�� ��.
	// 
	// Normalize�� ���� ���� = (X * Y) / ���� ������ ũ��
	// ���Ϳ��� = (X_2*Y_3 - X_3*Y_2, X_3*Y_1 - X_1*Y_3, X_1*Y_2 - X_2*Y_1)
	// �� ũ�� = SQRT((X_2*Y_3 - X_3*Y_2)^2 + (X_3*Y_1 - X_1*Y_3)^2 + (X_1*Y_2 - X_2*Y_1)^2))
	// �� ���ؽ��� ���� ������ ���Ͱ��� ���ǵ� �� ����.
	// �̶�, �� Vertices�� ����. ��ġ�� Vertice�� ����������. �̹� �����Ұ��. �� Vector���� ���հ��� ���ϰ� ��.
	// 
	// ���ϴ� ������� ��Ÿ���ٸ�, �ش� vertices�� Normalize���ָ� ��.
	// ������ ����.

	// Make IndicesNormal vector
	vector<VertexUVNormal>& vertices = mesh->GetVertices();
	vector<UINT>& indices = mesh->GetIndices();

	// ���߱�� mesh
	vector<VertexColor>& Lvertices = normalline->GetVertices();
	Lvertices.reserve(vertices.size() / 3 * 2);
	vector<UINT>& LIndices = normalline->GetIndices();
	LIndices.reserve(vertices.size() / 3 * 2);


	for (UINT i = 0; i < indices.size() / 3; i++)
	{
		// vertices������.
		Vector3 x = vertices[indices[i * 3 + 0]].pos;	// 0
		Vector3 y = vertices[indices[i * 3 + 1]].pos;	// 1
		Vector3 z = vertices[indices[i * 3 + 2]].pos;	// 2

		// �� ���� ����
		Vector3 normalVector = Vector3::ComputeNormal(x, y, z);
		Float3 normalfloat3 = normalVector.GetvalueFloat3();

		for (UINT c = 0; c < 3; c++) {
			if (vertices[indices[i * 3 + c]].normal.x != 0.0f &&
				vertices[indices[i * 3 + c]].normal.y != 0.0f &&
				vertices[indices[i * 3 + c]].normal.z != 0.0f)
				vertices[indices[i * 3 + c]].normal = normalfloat3;

			//���� �� �־��� ��� ��հ� ����.
			else
			{
				vertices[indices[i * 3 + c]].normal.x = (vertices[indices[i * 3 + c]].normal.x + normalfloat3.x) * 0.5;
				vertices[indices[i * 3 + c]].normal.y = (vertices[indices[i * 3 + c]].normal.y + normalfloat3.y) * 0.5;
				vertices[indices[i * 3 + c]].normal.z = (vertices[indices[i * 3 + c]].normal.z + normalfloat3.y) * 0.5;
			}
		}
		
		// ���߱� ���ؼ� �ڵ�����.
		Vector3 v1, v2;
		v1 = MakeNormalline(x, y, z).GetvalueFloat3();
		v2 = v1 + normalVector * 1;
		VertexColor n1, n2;
		n1.pos = v1.GetvalueFloat3();
		n2.pos = v2.GetvalueFloat3();

		n2.color = n1.color = { 1.0f, 1.0f, 1.0f, 1.0f };

		Lvertices.push_back(n1);
		Lvertices.push_back(n2);
	}

	// indices ����
	for (UINT i = 0; i < Lvertices.size() * 2; i++)
		LIndices.emplace_back(i);
}

Vector3 Terrain::MakeNormalline(Vector3 x, Vector3 y, Vector3 z)
{
	return { (x.x + y.x + z.x) / 3, (x.y + y.y + z.y) / 3, (x.z + y.z + z.z) / 3 };
}
