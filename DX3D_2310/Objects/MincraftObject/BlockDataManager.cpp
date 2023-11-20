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

	// ������� ġ��� ���� ����
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
//	// csv������ �����ͼ� ��������. ��絥���͸� map���·� �����ͼ� �����Ѵ�.
//	// unordered_map�� �� ȿ���� ����. �������.
//	ifstream loadFile("TextData/DataTable/BlockTable.csv");
//
//	string temp = {};
//
//	// ������� ġ��� ���� ����
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

