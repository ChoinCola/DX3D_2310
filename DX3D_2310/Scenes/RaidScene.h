#pragma once
class RaidScene : public Scene
{
public:
    RaidScene();
    ~RaidScene();

    void Update() override;
    void PreRender() override;
    void Render() override;
    void PostRender() override;
    void GUIRender() override;

private:
    TopviewTraveler* traveler;
    //Monster* monster;

    //Terrain* terrain;
};