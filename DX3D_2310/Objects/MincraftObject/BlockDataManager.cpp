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
	ifstream loadFile("TextData/DataTable/BlockTable.csv");

	string temp = {};

	// 헤더값을 치우기 위한 변수
	bool isFirstLine = true;

	while (loadFile.eof() == false)
	{
		getline(loadFile, temp);

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
		data.cash = stoi(datas[4]);
		data.itemtype = datas[5];
		data.damage = stoi(datas[6]);
		if (data.itemtype == "block")
			blockDatas[data.key] = data;
		else
			ObjectDatas[data.key] = data;
	}
}

//void BlockDataManager::LoadTable()
//{
//	FILE* file;
//	fopen_s(&file, "TextData/DataTable/BlockTable.csv", "r");
//
//	bool isFirstLine = true;
//	char str[128];
//	while (true)
//	{
//		fscanf_s(file, "%s\n", str, 128);
//		fgets(str, 128, file);
//
//		string temp = str;
//		if (isFirstLine == true)
//		{
//			isFirstLine = false;
//			continue;
//		}
//
//		vector<string> datas = SplitString(temp, ",");
//		BlockData data;
//		data.key = stoi(datas[0]);
//		data.name = datas[1];
//		data.modelname = datas[2];
//		data.uiimage = datas[3];
//		data.cash = stoi(datas[4]);
//		data.itemtype = datas[5];
//		data.damage = stoi(datas[6]);
//		if(data.itemtype == "block")
//			blockDatas[data.key] = data;
//		else
//			ObjectDatas[data.key] = data;
//	}
//}

//void BlockDataManager::LoadTable()
//{
//	// csv파일을 가져와서 긁은다음. 모든데이터를 map형태로 가져와서 저장한다.
//	// unordered_map이 더 효율이 좋다. 기억하자.
//	ifstream loadFile("TextData/DataTable/BlockTable.csv");
//
//	string temp = {};
//
//	// 헤더값을 치우기 위한 변수
//	bool isFirstLine = true;
//
//	while (loadFile.eof() == false)
//	{
//		loadFile >> temp;
//
//		if (isFirstLine)
//		{
//			isFirstLine = false;
//			continue;
//		}
//
//		vector<string> datas = SplitString(temp, ",");
//		BlockData data;
//		data.key = stoi(datas[0]);
//		data.name = datas[1];
//		data.modelname = datas[2];
//		data.uiimage = datas[3];
//		data.cash = stoi(datas[4]);
//		data.itemtype = datas[5];
//		data.damage = stoi(datas[6]);
//		if(data.itemtype == "block")
//			blockDatas[data.key] = data;
//		else
//			ObjectDatas[data.key] = data;
//	}
//}

