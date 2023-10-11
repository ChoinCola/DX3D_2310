#pragma once

class Cube
{
public:
	Cube(Float3 size = { 1, 1, 1});
	~Cube();

	void Update();
	void Render();

	void SetPos(float x, float y, float z) { pos = { x, y, z }; }
private:
	void Move();
	void Resize();
	void Rotation();
	void SRT();
private:
	VertexShader* vertexShader;
	PixelShader* pixelShader;

	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;

	MatrixBuffer* worldBuffer;

	vector<VertexColor> vertices;
	vector<UINT> indices;

	Matrix world;
	Matrix S, R, T;

	Float3 size = {};
	Float3 rotation = {};
	Float3 pos = {};
	Float3 move = {};
	Keyboard* key;
	float def = 0;
};