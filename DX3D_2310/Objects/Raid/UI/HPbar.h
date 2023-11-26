#pragma once

class HPbar
{
public:
	HPbar(string input, Transform* parent);
	~HPbar();

	void UpdateHPbar(float input, float inputmax);
private:
	string name;
	Quad* bar;
};