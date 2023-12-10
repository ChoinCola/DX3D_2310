#pragma once
#pragma once

struct SkillData
{
	int key;
	string name;
	int level;
	string icon;
	float power;
	float speed;
	float interval;
	int count;
	float lifeTime;
	bool through;
	float knockback;
};

class SurvivalDataManager : public Singleton<BlockDataManager>
{
private:
	friend class Singleton;
	SurvivalDataManager();
	~SurvivalDataManager();

public:
	void LoadTable();

	SkillData GetSkillData(int key) { return SkillDatas[key]; }

	int GetSkillSize() { return SkillDatas.size(); }

private:
	map<int, SkillData> SkillDatas;
};