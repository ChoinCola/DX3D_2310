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
	// 주변 Verteices랑 연결된 indices를 탐색한다.
	// 법선 벡터의 계산을 위해 각 삼각형의 모서리간의 벡터 두개를 구한다.
	// xyz가 있을때,
	// X = y-x;
	// Y = z-x;
	// 
	// 로 두개를 구함.
	// 이때 각각 X = (X_1, X_2, X_3)
	// Y = (Y_1, Y_2, Y_3)
	// 이라고 함.
	// 
	// Normalize된 면의 법선 = (X * Y) / 벡터 외적의 크기
	// 벡터외적 = (X_2*Y_3 - X_3*Y_2, X_3*Y_1 - X_1*Y_3, X_1*Y_2 - X_2*Y_1)
	// 의 크기 = SQRT((X_2*Y_3 - X_3*Y_2)^2 + (X_3*Y_1 - X_1*Y_3)^2 + (X_1*Y_2 - X_2*Y_1)^2))
	// 각 버텍스의 면의 법선은 위와같이 정의될 수 있음.
	// 이때, 각 Vertices를 계산시. 겹치는 Vertice가 존재함으로. 이미 존재할경우. 원 Vector와의 사잇값을 취하게 함.
	// 
	// 원하는 결과값이 나타났다면, 해당 vertices를 Normalize해주면 됨.
	// 수식을 구현.

	// Make IndicesNormal vector
	vector<VertexUVNormal>& vertices = mesh->GetVertices();
	vector<UINT>& indices = mesh->GetIndices();

	// 선긋기용 mesh
	vector<VertexColor>& Lvertices = normalline->GetVertices();
	Lvertices.reserve(vertices.size() / 3 * 2);
	vector<UINT>& LIndices = normalline->GetIndices();
	LIndices.reserve(vertices.size() / 3 * 2);


	for (UINT i = 0; i < indices.size() / 3; i++)
	{
		// vertices가져옴.
		Vector3 x = vertices[indices[i * 3 + 0]].pos;	// 0
		Vector3 y = vertices[indices[i * 3 + 1]].pos;	// 1
		Vector3 z = vertices[indices[i * 3 + 2]].pos;	// 2

		// 면 법선 생성
		Vector3 normalVector = Vector3::ComputeNormal(x, y, z);
		Float3 normalfloat3 = normalVector.GetvalueFloat3();

		for (UINT c = 0; c < 3; c++) {
			if (vertices[indices[i * 3 + c]].normal.x != 0.0f &&
				vertices[indices[i * 3 + c]].normal.y != 0.0f &&
				vertices[indices[i * 3 + c]].normal.z != 0.0f)
				vertices[indices[i * 3 + c]].normal = normalfloat3;

			//값이 들어가 있었을 경우 평균값 기입.
			else
			{
				vertices[indices[i * 3 + c]].normal.x = (vertices[indices[i * 3 + c]].normal.x + normalfloat3.x) * 0.5;
				vertices[indices[i * 3 + c]].normal.y = (vertices[indices[i * 3 + c]].normal.y + normalfloat3.y) * 0.5;
				vertices[indices[i * 3 + c]].normal.z = (vertices[indices[i * 3 + c]].normal.z + normalfloat3.y) * 0.5;
			}
		}
		
		// 선긋기 위해서 코드제작.
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

	// indices 설계
	for (UINT i = 0; i < Lvertices.size() * 2; i++)
		LIndices.emplace_back(i);
}

Vector3 Terrain::MakeNormalline(Vector3 x, Vector3 y, Vector3 z)
{
	return { (x.x + y.x + z.x) / 3, (x.y + y.y + z.y) / 3, (x.z + y.z + z.z) / 3 };
}
