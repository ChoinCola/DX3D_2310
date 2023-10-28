#include "Framework.h"
#include "Ball.h"

Ball::Ball(float radius, UINT stackCount, UINT sliceCount)
	: Sphere(radius, sliceCount, stackCount), SphereCollider(radius, stackCount)
{
}

Ball::~Ball()
{

}

void Ball::Update()
{

}

void Ball::Render()
{

}

void Ball::GUIRneder()
{

}
