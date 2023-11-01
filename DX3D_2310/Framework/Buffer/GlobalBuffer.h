#pragma once

class MatrixBuffer : public ConstBuffer
{
public:
	MatrixBuffer() : ConstBuffer(&matrix, sizeof(Matrix))
	{
		matrix = XMMatrixIdentity();
	}
	
	void Set(Matrix value)
	{
		matrix = XMMatrixTranspose(value);
	}

private:
	Matrix matrix;
};

class ViewBuffer : public ConstBuffer
{
private:
	struct Data
	{
		Matrix view;
		Matrix invView;
	};
public:
	ViewBuffer() : ConstBuffer(&data, sizeof(Data))
	{
		data.view = XMMatrixIdentity();
		data.invView = XMMatrixIdentity();
	}

	void Set(Matrix view, Matrix invView)
	{
		data.view = XMMatrixTranspose(view);
		data.invView = XMMatrixTranspose(invView);
	}

private:
	Data data;
};

class LightBuffer : public ConstBuffer
{
public:
	struct Light
	{
		Float4 color = { 1, 1, 1, 1 };

		Float3 direction = { 0, -1, 1 };
		int type;

		Float3 position = { 0, 0, 0 };
		float range = 100.0f;

		float inner = 55.0f;
		float outer = 70.0f;
		int isActive;
		float attentionIntensity = 1.5f;
	};
private:
	struct Data
	{
		Light lights[MAX_LIGHT];

		int lightCount = 1;
		Float3 ambientLight = { 0.1f, 0.1f, 0.1f };

		Float3 ambientCeil = { 0.1f, 0.1f, 0.1f };
		float padding;
	};

public:
	LightBuffer() : ConstBuffer(&data, sizeof(Data))
	{
	}

	Data* GetData() { return &data; }
	void Save() {}
	void Load() {}
private:
	Data data;
};

class MaterialBuffer : public ConstBuffer
{
public :
	struct Data
	{
		Float4 diffuse = { 1, 1, 1, 1 };
		Float4 specular = { 1, 1, 1, 1 };
		Float4 ambient = { 1, 1, 1, 1 };
		// 맞을때 빤짝거리는 것을 구현하기 위한 Data
		Float4 emissive = { 0, 0, 0, 0 };

		float shininess = 24.0f;
		float hasNormalMap = 0;
		float padding[2];
	};

public:
	MaterialBuffer() : ConstBuffer(&data, sizeof(Data))
	{
	}

	Data* GetData() { return &data; }

private:
	Data data;
};