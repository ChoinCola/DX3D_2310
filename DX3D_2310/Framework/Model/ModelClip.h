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

	// 노드 이름을 키로 하고 프레임 데이터를 데이터로 한다.
	unordered_map<string, KeyFrame*> keyFrames;
};