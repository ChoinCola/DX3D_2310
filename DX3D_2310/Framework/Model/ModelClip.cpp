#include "Framework.h"

ModelClip::~ModelClip()
{
	for (pair<string, KeyFrame*> keyFrame : keyFrames)
		delete keyFrame.second;
}
