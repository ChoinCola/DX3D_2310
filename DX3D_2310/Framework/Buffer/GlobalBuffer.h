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
private:
	struct Data
	{
		Float3 lightDirection = { 0, -1, 1 };

		float shininess = 24.0f;
	};

public:
	LightBuffer() : ConstBuffer(&data, sizeof(Data))
	{
	}

	Data* GetData() { return &data; }

private:
	Data data;
};