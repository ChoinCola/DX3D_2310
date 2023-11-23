#pragma once

class Camera : public Transform
{
public:
	Camera();
	~Camera();

	void Update();
	void GUIRender();

	void SetView();
	Ray ScreenPointToRay(Vector3 screenPoint);

	Matrix GetViewMatrix() { return view; }

	// 받은 위치좌표를 스크린좌표로 변환해주는 함수.
	Vector3 WorldToScreen(Vector3 worldPos);

	bool ContainPoint(Vector3 point); // 특정 점이 절도체 안에 들어오는지 아닌지 확인함.
	bool ContainSphere(Vector3 Cneter, float radius);

	void TargetOptionSave(string file);
	void TargetOptionLoad(string file);

	void SetTarget(Transform* target) { this->target = target; }

private:
	void FreeMode();
	void FollowMode();

	// 절도체의 육면을 알아내서. 평면방정식을 사용해 면 안에 있는지 알아낸다.
	void Frustum();

private:
	ViewBuffer* viewBuffer;
	Matrix view;
	Matrix projection;
	float moveSpeed = 50.0f;
	float rotSpeed = 10.0f;

	Vector3 prevMousePos;
	
	// TargetOption
	Transform* target = nullptr;

	float distance = 20.0f;
	float height = 20.0f;
	float moveDamping = 5.0f; // 따라가는 속도
	float rotDamping = 0.0f; // 회전에서 따라가는 속도
	float rotY = 0;			// 쿼터뷰로 하였을 때, 45도각도를 바라보고 싶을 수도있기 때문,

	float destRot = 0.0f;
	Vector3 destPos;

	Vector3 focusOffset;	// 카메라의 타겟 오프셋

	// 특정 타겟을 바라보는데 X축 앵글로만 안바라보게 하고싶을 수 있다.
	// 모델 유지 카메라만 이동하게
	bool isLookAtTargetX = true;
	bool isLookAtTargetY = true;

	// 회전행렬 저장
	Matrix rotMatrix;

	char file[128];

	Vector3 planes[6]; // 여섯개의 면을 만듬.
	float a, b, c, d;
};