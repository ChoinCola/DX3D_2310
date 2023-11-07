#pragma once

class SphereCollider : public Collider
{
public:
	SphereCollider(float radius = 1.0f, UINT stackCount = 8, UINT sliceCount = 16);
	~SphereCollider();
	// Collider을(를) 통해 상속됨
	bool IsRayCollision(IN const Ray& ray, OUT Contact* contact) override;
	bool IsBoxCollision(BoxCollider* collider) override;
	bool IsSphereCollision(SphereCollider* collider) override;
	bool IsCapsuleCollision(CapsuleCollider* collider) override;
	
	float& Getdistance() { return distance; }
	Vector3& GetHitpoint() { return hitpoint; }
	float Radius() { 
		return radius * max(GetGlobalScale().x, 
		max(GetGlobalScale().y, GetGlobalScale().z)); 
	}
private:
	void MakeMesh() override;

private:
	Vector3 hitpoint;
	float distance = 0;
	float radius;
	UINT stackCount, sliceCount;
};