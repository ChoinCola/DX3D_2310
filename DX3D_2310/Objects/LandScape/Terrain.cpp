#include "Framework.h"

Terrain::Terrain()
{
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
	
	__super::GUIRender();
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
