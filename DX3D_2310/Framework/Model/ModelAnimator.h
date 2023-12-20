#pragma once

// ModelAnimator Ŭ����: Model Ŭ������ ����Ͽ� ���� �ִϸ��̼��� �ٷ�� Ŭ����
class ModelAnimator : public Model
{
public:
	// Frame ����ü: �ִϸ��̼� ����� ���Ǵ� ������ ������ �����ϴ� ����ü
	struct Frame
	{
		int clip = 0;   // ���� ��� ���� Ŭ���� �ε���
		int curFrame = 0;   // ���� ������ �ε���
		float time = 0.0f;  // ���� �����ӿ����� ��� �ð�
		float scale = 1.0f;  // �ִϸ��̼� ��� �ӵ� ������ ������
	};

	struct Motion
	{
		float takeTime = 0.2f;
		float tweenTime = 0.0f;
		float runningTime = 0.0f;
		float duration = 0;

		Frame cur, next;

		Motion()
		{
			next.clip = -1.0f;
		}
	};

protected:
	// ClipTransform ����ü: �� ������ �� ���� ���� ��ȯ ��Ʈ������ �����ϴ� ����ü
	struct ClipTransform
	{
		Matrix transform[MAX_FRAME][MAX_BONE]; // �� ������ �� ���� ���� ��ȯ ��Ʈ���� �迭
	};

	// FrameBuffer Ŭ����: ��ӵ� ConstBuffer�� ����Ͽ� ������ �����͸� GPU�� �����ϴ� Ŭ����
	class FrameBuffer : public ConstBuffer
	{
	public:
		FrameBuffer() : ConstBuffer(&motion, sizeof(Motion)) {}; // ������ �����͸� ��ӵ� ConstBuffer�� ����

		Motion* GetData() { return &motion; } // ������ �����Ϳ� ���� ������ ��ȯ

	private:
		Motion motion; // ������ �����͸� �����ϴ� ����ü �ν��Ͻ�
	};

public:
	// ������: �� �̸��� ���ڷ� �޾� �ʱ�ȭ
	ModelAnimator(string name);
	// �Ҹ���: �������� �Ҵ�� �ڿ� ����
	~ModelAnimator();

	// ������Ʈ �Լ�: ������ �� �ִϸ��̼� ������Ʈ ����
	void Update();
	// ���� �Լ�: ���� ������
	void Render();
	// GUI ���� �Լ�: �ִϸ��̼� ���� GUI ������
	void GUIRender();

	// Ŭ�� �б� �Լ�: �ִϸ��̼� Ŭ���� �о�ͼ� ����
	void ReadClip(string clipName, UINT clipNum = 0, UINT count = 0);
	void PlayClip(int clip, float scale = 1.0f, float takeTime = 0.2f);

	// �ؽ�ó ���� �Լ�: �ؽ�ó�� �����Ͽ� ����
	void CreateTexture();

	Matrix GetTransformByNode(int nodeIndex);

	void SetRunAnimation(bool input) { IsPlay = input; }
	ModelClip* GetClip(UINT clip) { return clips[clip]; }

protected:
	// Ŭ�� ��ȯ ���� �Լ�: ������ �ε����� ���� Ŭ�� ��ȯ ��Ʈ���� ����
	void CreateClipTransform(UINT index);
	void UpdateFrame(Motion* motion);

	UINT GetMaxFrameNum();

protected:
	vector<ModelClip*> clips; // �� Ŭ������ �����ϴ� ����

	ClipTransform* clipTransforms; // ���� �� ��ȯ ��Ʈ���� �迭
	ClipTransform* nodeTransforms; // ���� �� ��ȯ ��Ʈ���� �迭

	ID3D11Texture2D* texture = nullptr; // �ؽ�ó�� ��Ÿ���� Direct3D �ؽ�ó �������̽�
	ID3D11ShaderResourceView* srv;      // ���̴����� ����ϴ� �ؽ�ó�� ��Ÿ���� �������̽�

	FrameBuffer* frameBuffer; // ��ӵ� ConstBuffer�� ����Ͽ� ������ �����͸� GPU�� �����ϴ� �ν��Ͻ�

	bool IsPlay = true; // �ڵ� �ִϸ��̼� ��� ���θ� ��Ÿ���� �÷���
};