#pragma once

class UnitCollider : public SphereCollider
{
public:
	UnitCollider(Transform* model);
	~UnitCollider();

	void Update();
	void Render();
	void ClickObject(bool input);

private:
	Transform* inputmodel;
};