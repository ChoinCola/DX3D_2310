#pragma once

class ModelAnimatorInstancingtoMap : public ModelAnimator
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
	ModelAnimatorInstancingtoMap(string name);
	~ModelAnimatorInstancingtoMap();

	void Update(pair<float, float> nowpos);
	void Render(pair<float, float> nowpos, Float2 size);

	void GUIRender();

	Transform* Add(pair<float, float> nowpos);

	void PlayClip(UINT instanceInex, int clip, float scale = 1.0f, float takeTime = 0.1f);

	Motion* GetMotion(UINT instanceIndex)
	{
		return &frameInstancingBuffer->GetData()->motions[instanceIndex];
	}

private:
	map<pair<float, float>, vector<Transform*>> transforms;
	map<pair<float, float>, InstanceData[MAX_INSTANCE]> instanceDatas;

	map<pair<float, float>, VertexBuffer*> instanceBuffer;
	FrameInstancingBuffer* frameInstancingBuffer;

	UINT drawCount = 0;
};