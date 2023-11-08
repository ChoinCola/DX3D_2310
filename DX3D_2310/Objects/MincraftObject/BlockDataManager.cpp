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
	// csv������ �����ͼ� ��������. ��絥���͸� map���·� �����ͼ� �����Ѵ�.
	// unordered_map�� �� ȿ���� ����. �������.
	ifstream loadFile("TextData/DataTable/BlockTable.csv");

	string temp = {};

	// ������� ġ��� ���� ����
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