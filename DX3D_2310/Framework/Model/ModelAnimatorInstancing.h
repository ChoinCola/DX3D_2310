#pragma once

class ModelAnimatorInstancing : public ModelAnimator
{
private:
	// FrameBuffer Ŭ����: ��ӵ� ConstBuffer�� ����Ͽ� ������ �����͸� GPU�� �����ϴ� Ŭ����
	class FrameInstancingBuffer : public ConstBuffer
	{
	public:
		struct Data
		{
			Motion motions[MAX_INSTANCE];
		};
	public:
		FrameInstancingBuffer() : ConstBuffer(&data, sizeof(Data)) {}; // ������ �����͸� ��ӵ� ConstBuffer�� ����

		Data* GetData() { return &data; } // ������ �����Ϳ� ���� ������ ��ȯ

	private:
		Data data; // ������ �����͸� �����ϴ� ����ü �ν��Ͻ�
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