#pragma once

// ModelAnimator 클래스: Model 클래스를 상속하여 모델의 애니메이션을 다루는 클래스
class ModelAnimator : public Model
{
protected:
	// ClipTransform 구조체: 각 프레임 및 본에 대한 변환 매트릭스를 저장하는 구조체
	struct ClipTransform
	{
		Matrix transform[MAX_FRAME][MAX_BONE]; // 각 프레임 및 본에 대한 변환 매트릭스 배열
	};

	// Frame 구조체: 애니메이션 재생에 사용되는 프레임 정보를 저장하는 구조체
	struct Frame
	{
		int clip = 0;        // 현재 재생 중인 클립의 인덱스
		int curFrame = 0;    // 현재 프레임 인덱스
		int nextcurFrame = 0;    // 다음 프레임 인덱스
		float time = 0;      // 현재 프레임에서의 경과 시간
		float scale = 1.0f;   // 애니메이션 재생 속도 조절용 스케일
		Float3 def;
	};

	// FrameBuffer 클래스: 상속된 ConstBuffer를 사용하여 프레임 데이터를 GPU에 전달하는 클래스
	class FrameBuffer : public ConstBuffer
	{
	public:
		FrameBuffer() : ConstBuffer(&frame, sizeof(Frame)) {}; // 프레임 데이터를 상속된 ConstBuffer에 전달

		Frame* GetData() { return &frame; } // 프레임 데이터에 대한 포인터 반환

	private:
		Frame frame; // 프레임 데이터를 저장하는 구조체 인스턴스
	};

public:
	// 생성자: 모델 이름을 인자로 받아 초기화
	ModelAnimator(string name);
	// 소멸자: 동적으로 할당된 자원 해제
	~ModelAnimator();

	// 업데이트 함수: 프레임 및 애니메이션 업데이트 수행
	void Update();
	// 렌더 함수: 모델을 렌더링
	void Render();
	// GUI 렌더 함수: 애니메이션 관련 GUI 렌더링
	void GUIRender();

	// 클립 읽기 함수: 애니메이션 클립을 읽어와서 저장
	void ReadClip(string clipName, UINT clipNum = 0, UINT count = 0);
	// 텍스처 생성 함수: 텍스처를 생성하여 저장
	void CreateTexture();

protected:
	// 클립 변환 생성 함수: 지정된 인덱스에 대한 클립 변환 매트릭스 생성
	void CreateClipTransform(UINT index);

protected:
	vector<ModelClip*> clips; // 모델 클립들을 저장하는 벡터

	ClipTransform* clipTransforms; // 원본 모델 변환 매트릭스 배열
	ClipTransform* nodeTransforms; // 보정 모델 변환 매트릭스 배열

	ID3D11Texture2D* texture = nullptr; // 텍스처를 나타내는 Direct3D 텍스처 인터페이스
	ID3D11ShaderResourceView* srv;      // 셰이더에서 사용하는 텍스처를 나타내는 인터페이스

	FrameBuffer* frameBuffer; // 상속된 ConstBuffer를 사용하여 프레임 데이터를 GPU에 전달하는 인스턴스

	bool AutoAnimation = false; // 자동 애니메이션 재생 여부를 나타내는 플래그
	float nowFrame = 0;         // 현재 프레임을 나타내는 변수
};