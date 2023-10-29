#include "Framework.h"
#include "Ball.h"

Ball::Ball(float radius, UINT stackCount, UINT sliceCount)
	: radius(radius), SphereCollider(radius, stackCount)
{
	sphere = new Sphere(radius, sliceCount, stackCount);
}

Ball::~Ball()
{
	SAFE_DELETE(sphere);
}

void Ball::Update()
{
	sphere->SetLocalPosition(this->GetLocalPosition());
	sphere->UpdateWorld();
	__super::UpdateWorld();
}

void Ball::Render()
{
	sphere->Render();
}

void Ball::GUIRneder()
{

}
