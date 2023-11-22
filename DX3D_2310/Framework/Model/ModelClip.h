#pragma once

class ModelClip
{
private:
	friend class ModelAnimator;

	ModelClip() = default;
	~ModelClip();

	void Init();
	void Excute();

	KeyFrame* GetKeyFrame(string boneName) { return keyFrames[boneName]; }

public:
	void SeEvent(Event event, float timeRatio) { events[timeRatio] = event; }
	// �ð��� �������ؼ� �ִ´�.

private:
	string name;

	UINT frameCount;
	float tickPerSecond, duration;
	float playTime = 0.0f;

	// ��� �̸��� Ű�� �ϰ� ������ �����͸� �����ͷ� �Ѵ�.
	unordered_map<string, KeyFrame*> keyFrames;

	map<float, Event> events;
	map<float, Event>::iterator eventIter;
};