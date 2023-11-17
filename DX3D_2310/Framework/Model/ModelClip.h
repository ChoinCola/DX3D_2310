#pragma once

class ModelClip
{
private:
	friend class ModelAnimator;

	ModelClip() = default;
	~ModelClip();

	KeyFrame* GetKeyFrame(string boneName) { return keyFrames[boneName]; }

private:
	string name;

	UINT frameCount;
	float tickPerSecond;

	// ��� �̸��� Ű�� �ϰ� ������ �����͸� �����ͷ� �Ѵ�.
	unordered_map<string, KeyFrame*> keyFrames;
};