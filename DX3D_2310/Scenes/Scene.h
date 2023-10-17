#pragma once

// �������̽� ������� �����Ѵ�.
// �����Լ� ?
/*
	
*/
class Scene
{
public:
	virtual ~Scene() = default;

	virtual void Update() = 0; // ���� �����Լ�.
	// �ڽ�Class���� ������ override���� ������ �ȵǰ� �������ִ°�.
	// ������ ���ɽ������θ� �����.
	virtual void PreRender() = 0;	// ����Ÿ�� �� �� �⺻������ ����ϴ� ���� �̸� ���ִ°�.
	virtual void Render() = 0;		// ��ü ����.
	virtual void PostRender() = 0;	// ��ó��
	virtual void GUIRender() = 0;

	virtual void Start() {}
	virtual void End() {}
};