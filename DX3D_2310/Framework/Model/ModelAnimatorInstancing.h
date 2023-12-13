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
	void PreRender();
	void Render();
	void Render(VertexBuffer* input);

	void GUIRender();

	Transform* Add();
	void TransformUnActive(UINT input);
	UINT GetLastObjectNum();
	void SetIsPreRneder(UINT input, bool isprerender) { transforms[input]->SetPreRender(isprerender); }
	void PlayClip(UINT instanceInex, int clip, float scale = 1.0f, float takeTime = 0.1f);
	UINT GetPlayClip(UINT instanceInex);

	Motion* GetMotion(UINT instanceIndex)
	{
		return &frameInstancingBuffer->GetData()->motions[instanceIndex];
	}

	UINT GetClipSize() { return clips.size(); }

private:
	vector<Transform*> transforms;

	InstanceData instanceDatas[MAX_INSTANCE];
	InstanceData PreRenderinstanceDatas[MAX_INSTANCE];

	VertexBuffer* instanceBuffer;
	VertexBuffer* instancePrerenderBuffer;

	FrameInstancingBuffer* frameInstancingBuffer;

	UINT drawCount = 0;
	UINT PreRenderdrawCount = 0;

	UINT LastInputnum = 0;
};