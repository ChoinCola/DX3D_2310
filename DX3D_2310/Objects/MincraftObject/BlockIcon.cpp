#include "Framework.h"

BlockIcon::BlockIcon(BlockData data)
	:	Button(ToWString(data.uiimage)), data(data)
{
}

BlockIcon::~BlockIcon()
{
}

void BlockIcon::OnClick()
{

}
