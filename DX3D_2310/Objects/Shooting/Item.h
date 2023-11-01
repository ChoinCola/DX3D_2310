#pragma once
#include "Objects/Shooting/ShootingPlayer.h"

class Item : public SphereCollider
{
public:
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void GetItem(ShootingPlayer* player) = 0;
private:
	float RotationSpeed = 1.0f;
	Cube* Object;
};