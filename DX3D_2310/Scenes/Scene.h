#pragma once

// 인터페이스 방식으로 구현한다.
// 가상함수 ?
/*
	
*/
class Scene
{
public:
	virtual ~Scene() = default;

	virtual void Update() = 0; // 순수 가상함수.
	// 자식Class에서 무조건 override하지 않으면 안되게 정의해주는것.
	// 무조건 업케스팅으로만 써야함.
	virtual void PreRender() = 0;	// 렌더타겟 쓸 때 기본적으로 써야하는 것을 미리 해주는것.
	virtual void Render() = 0;		// 객체 렌더.
	virtual void PostRender() = 0;	// 후처리
	virtual void GUIRender() = 0;

	virtual void Start() {}
	virtual void End() {}
};