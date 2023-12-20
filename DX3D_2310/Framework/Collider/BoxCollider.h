#pragma once

class BoxCollider : public Collider
{
public:
	struct ObbDesc
	{
		Vector3 pos; // 박스의 중간위치
		Vector3 axis[3]; // xyz 3개의 축
		Vector3 halfSize; // 크기의 반이 필요하다.
	};
	
public:
	BoxCollider(Vector3 size = { 1, 1, 1 });
	~BoxCollider() = default;
	virtual void GUIRender() override;
	// Collider을(를) 통해 상속됨
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
	// 특정축 기준으로 나누어져 있는지 아닌지 확인

private:
	Vector3 size;
};