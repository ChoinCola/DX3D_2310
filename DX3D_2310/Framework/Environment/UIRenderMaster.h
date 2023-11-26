#pragma once

class UIRenderMaster : public Singleton<UIRenderMaster>
{
private:
    friend class Singleton;

    UIRenderMaster();
    ~UIRenderMaster();

public:
    void InputUI2D(string UI);
    void InputUI2D(string UI, wstring Route);
    Quad* GetUI2D(string UI);
    void DeleteUI2D(string UI);

public:
    void InputUI3D(string UI);
    void InputUI3D(string UI, wstring Route);
    Quad* GetUI3D(string UI);
    void DeleteUI3D(string UI);

public:
    void Set2DUIScaleAtSort(string UI, UINT sort, float x, float y, Vector3 DefaltPos, Vector3 DefaltScale);
    void Set3DUIScaleAtSort(string UI, UINT sort, float x, float y, Vector3 DefaltPos, Vector3 DefaltScale);

    void Update();
    void Update2D();
    void Update3D();

    void Render();
    void PostRender();
    void GUIRender();

private:
    unordered_map<string, Quad*> _3DUImap;
    unordered_map<string, Quad*>::iterator _3DUImapIter;

    unordered_map<string, Quad*> UImap;
    unordered_map<string, Quad*>::iterator UImapIter;
};