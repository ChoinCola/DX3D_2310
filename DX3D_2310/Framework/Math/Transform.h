#pragma once

class Transform
{
public:
	Transform();

	void GUIRender();

	void UpdateWorld();

	void Translate(Vector3 direction);
	void Rotate(Vector3 direction);
	void SetRotate(Vector3 direction);

	Vector3 GetLocalPosition() const { return localPosition; }
	Vector3 GetLocalRotation() const { return localRotation; }
	Vector3 GetLocalScale() const { return localScale; }

	void SetLocalPosition(const Vector3& position) { localPosition = position; }
	void SetLocalRotation(const Vector3& rotation) { localRotation = rotation; }
	void SetLocalScale(const Vector3& scale) { localScale = scale; }

	Transform* GetSelf() { return this; }
	Transform* GetParent() { return parent; }
	void SetParent(Transform* transform) { parent = transform; }
	void SetPivot(Vector3 pivot) { this->pivot = pivot; }
	void SetTag(string tag) { this->tag = tag; }


protected:
	string tag;

	Vector3 localPosition = {};
	Vector3 localRotation = {};
	Vector3 localScale = { 1, 1, 1 }; // 배율이기 떄문에 0이 되면 안됨.

	Matrix world;

private:
	Transform* parent = nullptr;

	Vector3 pivot;
};