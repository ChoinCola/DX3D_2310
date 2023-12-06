#pragma once

class TopViewWeaponFireBall : public TopViewWeapon
{
public:
	TopViewWeaponFireBall(SkillData data, Transform* player);
	~TopViewWeaponFireBall();

	// TopViewWeapon을(를) 통해 상속됨
	void Update() override;
	void Render() override;

private:
	float time = 0.0f;
};