#pragma once

class Tree : public SphereCollider
{
public:
	Tree();
	~Tree();

	void Update();

	void Fire(Vector3 pos, Vector3 direction);
private:
	Transform* meshtransform;
};