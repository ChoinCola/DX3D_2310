#pragma once

class Vector3
{
public:
	Vector3(float x = 0.0f, float y = 0.0f, float z = 0.0f)
		: value(XMVectorSet(x, y, z, 0))
	{}
	Vector3(Float3 value) : value(XMLoadFloat3(&value)) {}
	Vector3(Vector4 value) : value(value) {}

	operator Vector4() { return value; }

	operator Float4()
	{
		Float4 result;
		XMStoreFloat4(&result, value);

		return result;
	}
	operator Float3()
	{
		Float3 result;
		XMStoreFloat3(&result, value);

		return result;
	}

	operator Float2()
	{
		Float2 result;
		XMStoreFloat2(&result, value);

		return result;
	}

	operator string()
	{
		string result;
		result = "X : " + to_string(GetX()) + " Y : " + to_string(GetY()) + " Z : " + to_string(GetZ());

		return result;
	}


	void SetX(const float& x) { value = XMVectorSetX(value, x); }
	void SetY(const float& y) { value = XMVectorSetY(value, y); }
	void SetZ(const float& z) { value = XMVectorSetZ(value, z); }
	void SetW(const float& z) { value = XMVectorSetW(value, z); }

	float GetX() const { return XMVectorGetX(value); }
	float GetY() const { return XMVectorGetY(value); }
	float GetZ() const { return XMVectorGetZ(value); }
	float GetW() const { return XMVectorGetW(value); }

	// 함수를 끌어다가 쓰는것.
	__declspec(property(get = GetX, put = SetX)) float x;
	__declspec(property(get = GetY, put = SetY)) float y;
	__declspec(property(get = GetZ, put = SetZ)) float z;

	float operator[](const UINT& index) const
	{
		switch (index)
		{
		case 0:
			return GetX();
		case 1:
			return GetY();
		case 2:
			return GetZ();
		}
	}

	Vector3 operator+(const Vector3& v) const { return value + v.value; }
	Vector3 operator-(const Vector3& v) const { return value - v.value; }

	void operator += (const Vector3& v) { value += v.value; }
	void operator -= (const Vector3& v) { value -= v.value; }

	Vector3 operator*(const float& v) const { return value * v; }
	Vector3 operator/(const float& v) const { return value / v; }

	bool operator == (const Vector3& v) const {
		return XMVector3Equal(value, v.value);
	}
	bool operator != (const Vector3& v) const {
		return !XMVector3Equal(value, v.value);
	}
	friend void operator +=(Float3& v1, const Vector3& v2)
	{
		Vector4 temp = XMLoadFloat3(&v1) + v2.value;
		XMStoreFloat3(&v1, temp);
	}

	friend void operator -=(Float3& v1, const Vector3& v2)
	{
		Vector4 temp = XMLoadFloat3(&v1) - v2.value;
		XMStoreFloat3(&v1, temp);
	}

	Vector3 GetNormalized() const { return XMVector3Normalize(value); }
	void Normalized() { value = XMVector3Normalize(value); }

	float Length() const { return XMVectorGetX(XMVector3Length(value)); }

	Vector4* GetValue() { return &value; }
	const Float3 GetvalueFloat3() { return { this->GetX(),this->GetY(),this->GetZ() }; }
	const Float4 GetvalueFloat4() { 
		XMFLOAT4 result;
		DirectX::XMStoreFloat4(&result, value);
		return result;
	}

	// 외적 함수
	static Vector3 Cross(const Vector3& vec1, const Vector3& vec2) { return XMVector3Cross(vec1.value, vec2.value); }
	static float Dot(const Vector3& vec1, const Vector3& vec2) { return XMVectorGetX(XMVector3Dot(vec1.value, vec2.value)); }
	static Vector3 ComputeNormal(Vector3& vec1, Vector3& vec2, Vector3& vec3) {
		//앞면 법선함수.
		Vector3 u = vec2 - vec1;
		Vector3 v = vec3 - vec1;
		return XMVector3Normalize(Cross(u, v));
	}

	void XMMATRIXToXMFLOAT3(const XMMATRIX& matrix)
	{
		value = XMVectorSet(matrix.r[3].m128_f32[0], matrix.r[3].m128_f32[1], matrix.r[3].m128_f32[2], 0);
	}

public:
	static Vector3 Zero() { return Vector3(); }
	static Vector3 One() { return Vector3(1, 1, 1); }
	static Vector3 Right() { return Vector3(1, 0, 0); }
	static Vector3 Left() { return Vector3(-1, 0, 0); }
	static Vector3 Up() { return Vector3(0, 1, 0); }
	static Vector3 Down() { return Vector3(0, -1, 0); }
	static Vector3 Forward() { return Vector3(0, 0, 1); }
	static Vector3 Back() { return Vector3(0, 0, -1); }

private:
	Vector4 value;	// 라이브러리의 도움을 받기 위해 기본 자료형을 이렇게 짠다.
};