#pragma once

class TopViewWeapon
{
public:
	TopViewWeapon(SkillData data, Transform* player) : data(data), player(player) {}
	virtual ~TopViewWeapon() = default;
	virtual void Update() = 0;
	virtual void Render() = 0;

protected:
	SkillData data;
	Transform* player;
};