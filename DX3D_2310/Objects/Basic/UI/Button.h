#pragma once

class Button : public Quad
{
protected:
	enum State
	{
	//	기본	  다운	마우스 포인터가 위에 있을때
		NONE, DOWN, OVER
	};

	const Float4 NONE_COLOR = { 1.0f, 1.0f, 1.0f, 1.0f };
	const Float4 DOWN_COLOR = { 0.5f, 0.5f, 0.5f, 0.5f };
	const Float4 OVER_COLOR = { 0.9f, 0.9f, 0.9f, 0.9f };

public:
	Button(wstring textureFile);
	Button(Float2 size = {32, 32});
	~Button() = default;

	void Update();

	void SetEvent(Event event) { this->event = event; }
	void SetPramEvnet(ParamEvent paramevet, void* object)
	{
		this->paramevent = paramevet;
		this->object = object;
	}


protected:
	State state = NONE;
	Event event = nullptr;
	ParamEvent paramevent = nullptr;

	void* object = nullptr;
	bool isDownCheck = false;
};