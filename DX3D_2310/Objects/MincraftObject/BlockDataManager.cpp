#include "Framework.h"
#include "BlockDataManager.h"

BlockDataManager::BlockDataManager()
{
}

BlockDataManager::~BlockDataManager()
{
}

void BlockDataManager::LoadTable()
{
	// csv파일을 가져와서 긁은다음. 모든데이터를 map형태로 가져와서 저장한다.
	// unordered_map이 더 효율이 좋다. 기억하자.
	ifstream loadFile("TextData/DataTable/BlockTable.csv");

	string temp = {};

	// 헤더값을 치우기 위한 변수
	bool isFirstLine = true;

	while (loadFile.eof() == false)
	{
		loadFile >> temp;

		if (isFirstLine)
		{
			isFirstLine = false;
			continue;
		}

		vector<string> datas = SplitString(temp, ",");
		BlockData data;
		data.key = stoi(datas[0]);
		data.name = datas[1];
		data.modelname = datas[2];
		data.uiimage = datas[3];

		blockDatas[data.key] = data;
	}
}