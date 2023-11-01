#pragma once
#include "Objects/Shooting/ShootingPlayer.h"


class Item : public SphereCollider
{
public:
	enum
	{
		None,
		Ammo,
		Gold
	};

public:
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void GetItem(int& player) = 0;
	bool GetIsRender() { return IsRender; }
	const int GetItemType() { return ItemType; }
protected:
	int ItemType = None;
	bool IsRender = true;
	float RotationSpeed = 1.0f;
	float Yoing = 0.0f;
	Cube* Object;
};