#pragma once

class CapsuleCollider : public Collider
{
public:
	CapsuleCollider(float radius = 1.0f, float height = 2.0f, UINT stackCount = 9, UINT sliceCount = 16);
	~CapsuleCollider() = default;

	// Collider을(를) 통해 상속됨
	bool IsRayCollision(IN const Ray& ray, OUT Contact* contact) override;
	bool IsBoxCollision(BoxCollider* collider) override;
	bool IsSphereCollision(SphereCollider* collider) override;
	bool IsCapsuleCollision(CapsuleCollider* collider) override;
	

	float Radius() {
		return radius * max(GetGlobalScale().x,
			max(GetGlobalScale().y, GetGlobalScale().z));
	}

	float Height() { return height * GetGlobalScale().y; }

private:
	void MakeMesh() override;

private:
	Vector3 hitpoint;
	float distance = 0;
	float radius, height;
	UINT stackCount, sliceCount;
};