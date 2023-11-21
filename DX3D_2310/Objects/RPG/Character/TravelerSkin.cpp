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
		PlayClip(0);
		break;

	case Front :
		PlayClip(1);

		break;

	case Right :
		PlayClip(3);

		break;

	case Left :
		PlayClip(5);
		break;

	case Back :
		PlayClip(7);
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
