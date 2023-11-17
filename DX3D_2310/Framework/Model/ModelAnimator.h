#pragma once

// ModelAnimator Ŭ����: Model Ŭ������ ����Ͽ� ���� �ִϸ��̼��� �ٷ�� Ŭ����
class ModelAnimator : public Model
{
protected:
	// ClipTransform ����ü: �� ������ �� ���� ���� ��ȯ ��Ʈ������ �����ϴ� ����ü
	struct ClipTransform
	{
		Matrix transform[MAX_FRAME][MAX_BONE]; // �� ������ �� ���� ���� ��ȯ ��Ʈ���� �迭
	};

	// Frame ����ü: �ִϸ��̼� ����� ���Ǵ� ������ ������ �����ϴ� ����ü
	struct Frame
	{
		int clip = 0;        // ���� ��� ���� Ŭ���� �ε���
		int curFrame = 0;    // ���� ������ �ε���
		int nextcurFrame = 0;    // ���� ������ �ε���
		float time = 0;      // ���� �����ӿ����� ��� �ð�
		float scale = 1.0f;   // �ִϸ��̼� ��� �ӵ� ������ ������
		Float3 def;
	};

	// FrameBuffer Ŭ����: ��ӵ� ConstBuffer�� ����Ͽ� ������ �����͸� GPU�� �����ϴ� Ŭ����
	class FrameBuffer : public ConstBuffer
	{
	public:
		FrameBuffer() : ConstBuffer(&frame, sizeof(Frame)) {}; // ������ �����͸� ��ӵ� ConstBuffer�� ����

		Frame* GetData() { return &frame; } // ������ �����Ϳ� ���� ������ ��ȯ

	private:
		Frame frame; // ������ �����͸� �����ϴ� ����ü �ν��Ͻ�
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
	// �ؽ�ó ���� �Լ�: �ؽ�ó�� �����Ͽ� ����
	void CreateTexture();

protected:
	// Ŭ�� ��ȯ ���� �Լ�: ������ �ε����� ���� Ŭ�� ��ȯ ��Ʈ���� ����
	void CreateClipTransform(UINT index);

protected:
	vector<ModelClip*> clips; // �� Ŭ������ �����ϴ� ����

	ClipTransform* clipTransforms; // ���� �� ��ȯ ��Ʈ���� �迭
	ClipTransform* nodeTransforms; // ���� �� ��ȯ ��Ʈ���� �迭

	ID3D11Texture2D* texture = nullptr; // �ؽ�ó�� ��Ÿ���� Direct3D �ؽ�ó �������̽�
	ID3D11ShaderResourceView* srv;      // ���̴����� ����ϴ� �ؽ�ó�� ��Ÿ���� �������̽�

	FrameBuffer* frameBuffer; // ��ӵ� ConstBuffer�� ����Ͽ� ������ �����͸� GPU�� �����ϴ� �ν��Ͻ�

	bool AutoAnimation = false; // �ڵ� �ִϸ��̼� ��� ���θ� ��Ÿ���� �÷���
	float nowFrame = 0;         // ���� �������� ��Ÿ���� ����
};