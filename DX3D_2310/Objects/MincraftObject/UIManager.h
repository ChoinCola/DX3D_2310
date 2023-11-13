#pragma once

class UIManager : public Singleton<UIManager>
{
private:
	friend class Singleton;

	UIManager();
	~UIManager();

public:
	bool IsMouseOver();
	void AddUI(Quad* ui) { totalUI.push_back(ui); }

private:
	vector<Quad*> totalUI;
};