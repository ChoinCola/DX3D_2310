#pragma once

class ModelAnimatorInstancing : public ModelAnimator
{
private:
	// FrameBuffer 클래스: 상속된 ConstBuffer를 사용하여 프레임 데이터를 GPU에 전달하는 클래스
	class FrameInstancingBuffer : public ConstBuffer
	{
	public:
		struct Data
		{
			Motion motions[MAX_INSTANCE];
		};
	public:
		FrameInstancingBuffer() : ConstBuffer(&data, sizeof(Data)) {}; // 프레임 데이터를 상속된 ConstBuffer에 전달

		Data* GetData() { return &data; } // 프레임 데이터에 대한 포인터 반환

	private:
		Data data; // 프레임 데이터를 저장하는 구조체 인스턴스
	};

public:
	ModelAnimatorInstancing(string name);
	~ModelAnimatorInstancing();

	void Update();
	void Render();
	void Render(VertexBuffer* input);

	void GUIRender();

	Transform* Add();

	void SetTransforms(vector<Transform*>* transform);
	void PlayClip(UINT instanceInex, int clip, float scale = 1.0f, float takeTime = 0.1f);

	Motion* GetMotion(UINT instanceIndex)
	{
		return &frameInstancingBuffer->GetData()->motions[instanceIndex];
	}

private:
	vector<Transform*> transforms;
	vector<Transform*>* inputTransforms = nullptr;
	InstanceData instanceDatas[MAX_INSTANCE];

	VertexBuffer* instanceBuffer;
	FrameInstancingBuffer* frameInstancingBuffer;

	UINT drawCount = 0;
};