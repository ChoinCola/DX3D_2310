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
	void SetEvent(Event event, float timeRatio) { events[timeRatio] = event; }

private:
	string name;

	UINT frameCount;
	float tickPerSecond, duration;
	float playTime = 0.0f;

	// 노드 이름을 키로 하고 프레임 데이터를 데이터로 한다.
	unordered_map<string, KeyFrame*> keyFrames;

	map<float, Event> events;
	map<float, Event>::iterator eventIter;
};