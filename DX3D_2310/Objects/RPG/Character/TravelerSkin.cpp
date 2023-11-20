#include "Framework.h"
#include "TravelerSkin.h"

TravelerSkin::TravelerSkin(Collider* playerDefalt)
	: ModelAnimator("Traveler"), DefaltPosition(playerDefalt)
{
	ReadClip("Idle");
	ReadClip("Walk");
	ReadClip("IdleToWalkForward");
	ReadClip("Right_Walk");
	ReadClip("IdleTwistR");
	ReadClip("Left_Walk");
	ReadClip("IdleTwistL");
	ReadClip("Back_Walk");
	//ReadClip("Flair");
	CreateTexture();
}

void TravelerSkin::SetAnimation(UINT input)
{
	switch (input)
	{
	case Idle :
		frameBuffer->GetData()->nextclip = 0;
		break;

	case Front :
		frameBuffer->GetData()->nextclip = 1;
		break;

	case Right :
		frameBuffer->GetData()->nextclip = 3;
		break;

	case Left :
		frameBuffer->GetData()->nextclip = 5;
		break;

	case Back :
		frameBuffer->GetData()->nextclip = 7;
		break;

	}
}

void TravelerSkin::MoveFront()
{

}

void TravelerSkin::MoveLeft()
{
}

void TravelerSkin::MoveRight()
{
}

void TravelerSkin::MoveBack()
{
}
