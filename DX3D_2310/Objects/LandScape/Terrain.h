#pragma once

class Terrain : public GameObject
{
private:
	typedef VertexUVNormal VertexType;
public:
	Terrain();
	Terrain(const wstring hightmap, const float hight = 1, bool tile = false, bool flip = false);

	~Terrain();

	void Render();
	void GUIRender();

	// 현재 위치에서 그라운드 높이값을 구하는 함수
	Vector3 GetOnGrondPosition(const Vector3 ObjectPos, const Vector3 correction = {0, 0, 0});
	// 현재 위치에서 중력가속도 구하는 함수
	Vector3 GetOngravityAcceleration(const Vector3 ObjectPos, const Vector3 correction = { 0, 0, 0 });

	bool ChackOnGround(const Vector3 ObjectPos);
	int GetWidth() { return width; }
	int GetHeight() { return height; }

private:
	pair<Vector3, Vector3> FindLowPos(Vector3 e0, Vector3 e1, Vector3 e2, Vector3 e3);
	void MakeMesh(bool tile = false, bool flip = false);
	void MakeNormal(bool Flip = false);

private:
	UINT width = 10, height = 10;
	Mesh<VertexType>* mesh;
	Mesh<VertexColor>* normalline;
	RasterizerState* RSset;
	Texture* heightMap;
	int x = 0, z = 0;
	double y = 0;
	double D = 0;
	float hight = 1;


	Vector3 v;
	bool IsPrintNormalLine = false;
	bool IsMapLine = false;
	bool ChackOne = false;
};