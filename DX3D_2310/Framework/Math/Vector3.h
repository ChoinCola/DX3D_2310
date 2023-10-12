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

	void SetX(const float& x) { value = XMVectorSetX(value, x); }
	void SetY(const float& y) { value = XMVectorSetY(value, y); }
	void SetZ(const float& z) { value = XMVectorSetZ(value, z); }

	float GetX() const { return XMVectorGetX(value); }
	float GetY() const { return XMVectorGetY(value); }
	float GetZ() const { return XMVectorGetZ(value); }

	// 함수를 끌어다가 쓰는것.
	__declspec(property(get = GetX, put = SetX)) float x;
	__declspec(property(get = GetY, put = SetY)) float y;
	__declspec(property(get = GetZ, put = SetZ)) float z;

	void operator += (const Vector3& v) { value += v.value; }
	void operator -= (const Vector3& v) { value -= v.value; }


private:
	Vector4 value;	// 라이브러리의 도움을 받기 위해 기본 자료형을 이렇게 짠다.
};