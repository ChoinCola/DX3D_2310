#pragma once

class SceneManager : public Singleton<SceneManager>
{

	friend class Singleton;

	SceneManager();
	~SceneManager();
public:
	void Update();
	void PreRender();
	void Render();
	void PostRender();
	void GUIRender();

	void Create(string key, Scene* scene);

	Scene* Add(string key);
	void Remove(string key);
private:
	void AddScene();
	void RemoveScene();

private:
	unordered_map<string, Scene*> scenes;
	
	list<Scene*> curScenes;
	// �߰��� ���� �����Ұ�� ����ϰ� �Ϸ���.

	vector<string> addScenes, removeScenes;
	// �����Ǿ�� �� ���� ���� ������ ������ �����ϰ� �ϱ����ؼ�
};