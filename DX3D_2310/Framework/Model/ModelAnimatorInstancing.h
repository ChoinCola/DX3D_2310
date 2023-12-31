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