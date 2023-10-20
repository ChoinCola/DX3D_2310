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

class LightBuffer : public ConstBuffer
{
private:
	struct Data
	{
		Float3 lightDirection = { 0, -1, 1 };
		float padding;
	};
public:
	LightBuffer() : ConstBuffer(&data, sizeof(Data))
	{
	}
	Data* GetData() { return &data; }

private:
	Data data;
};