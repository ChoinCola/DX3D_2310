#include "Framework.h"

SurvivalDataManager::SurvivalDataManager()
{
}

SurvivalDataManager::~SurvivalDataManager()
{
}

void SurvivalDataManager::LoadTable()
{
	ifstream loadFile("TextData/DataTable/SkillTable.csv");

	string temp = {};

	// 헤더값을 치우기 위한 변수
	bool isFirstLine = true;

	while (getline(loadFile, temp))
	{
		//getline(loadFile, temp);

		if (isFirstLine)
		{
			isFirstLine = false;
			continue;
		}

		vector<string> datas = SplitString(temp, ",");
		SkillData data;
		data.key =			stoi(datas[0]);	//	int key;
		data.name =			datas[1];	//	string name;
		data.level =		stoi(datas[2]);	//	int level;
		data.icon =			datas[3];	//	string icon;
		data.power =		stof(datas[4]);	//	float power;
		data.speed =		stof(datas[5]);	//	float speed;
		data.interval =		stof(datas[6]);	//	float interval;
		data.count =		stoi(datas[7]);	//	int count;
		data.lifeTime =		stof(datas[8]);	//	float lifeTime;
		data.through =		stoi(datas[9]);	//	bool through;
		data.knockback =	stof(datas[10]);//	float knockback;

		SkillDatas[data.key] = data;
	}
}