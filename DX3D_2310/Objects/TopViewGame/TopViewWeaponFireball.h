#pragma once

class TopViewWeaponFireBall : public TopViewWeapon
{
public:
	TopViewWeaponFireBall(SkillData data, Transform* player);
	~TopViewWeaponFireBall();

	// TopViewWeapon��(��) ���� ��ӵ�
	void Update() override;
	void Render() override;

private:
	float time = 0.0f;
};