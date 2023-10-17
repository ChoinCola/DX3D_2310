#pragma once

class GameBox : public Transform
{
public:
	GameBox(float slice = 1, float num = 0, Float3 size = { 1, 1, 1 }, Vector3 pos = { 0, 0, 0 });
	~GameBox();

	void Update();
	void Render();
	void SetImage(wstring imagenow);
private:
	Float2 uv = { 0, 0 };

	Material* material;
	Mesh<VertexUV>* mesh;

	MatrixBuffer* worldBuffer;

	ID3D11ShaderResourceView* srv;
};