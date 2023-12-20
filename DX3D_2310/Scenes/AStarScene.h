#pragma once

class AStarScene : public Scene
{
public:
	AStarScene();
	~AStarScene();


	// Scene을(를) 통해 상속됨
	void Update() override;

	void PreRender() override;

	void Render() override;

	void PostRender() override;

	void GUIRender() override;

private:
	Terrain* terrain;
	AStar* astar;
	Lion* lion;
};