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
	// 중간에 신을 빼야할경우 깔끔하게 하려고.

	vector<string> addScenes, removeScenes;
	// 삭제되어야 할 씬을 따로 빼놓고 끝나고 삭제하게 하기위해서
};