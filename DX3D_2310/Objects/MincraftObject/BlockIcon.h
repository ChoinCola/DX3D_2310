#pragma once
class BlockIcon : public Button
{
	BlockIcon(BlockData data);
	~BlockIcon();

	void OnClick();

private:
	BlockData data;

};