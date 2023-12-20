#pragma once

class BoxCollider : public Collider
{
public:
	struct ObbDesc
	{
		Vector3 pos; // �ڽ��� �߰���ġ
		Vector3 axis[3]; // xyz 3���� ��
		Vector3 halfSize; // ũ���� ���� �ʿ��ϴ�.
	};
	
public:
	BoxCollider(Vector3 size = { 1, 1, 1 });
	~BoxCollider() = default;
	virtual void GUIRender() override;
	// Collider��(��) ���� ��ӵ�
	bool IsRayCollision(IN const Ray& ray, OUT Contact* contact) override;
	bool IsBoxCollision(BoxCollider* collider) override;
	bool IsSphereCollision(SphereCollider* collider) override;
	bool IsCapsuleCollision(CapsuleCollider* collider) override;

	void GetObb(ObbDesc& obbDesc);
	Vector3 Getsize() { return size * GetLocalScale() * 0.5; }

	void BoxSave();
	void BoxLoad();
private:
	void MakeMesh() override;
	void UpdateMesh();

	bool IsSeperateAxis(const Vector3 D, const Vector3 axis, 
		const ObbDesc& box1, const ObbDesc& box2); 
	// Ư���� �������� �������� �ִ��� �ƴ��� Ȯ��

private:
	Vector3 size;
};