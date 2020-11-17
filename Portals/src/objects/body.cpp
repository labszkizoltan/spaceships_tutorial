
#include "body.h"

void Body::Accelerate(float deltav)
{
	velocity += deltav * orientation.f3;
}

void Body::Deccelerate(float deltav)
{
	velocity -= deltav * orientation.f3;
}

void Body::MoveForward(float distance)
{
	location += distance * orientation.f3;
}

void Body::MoveBackward(float distance)
{
	location -= distance * orientation.f3;
}

void Body::MoveLeft(float distance)
{
	location -= distance * orientation.f1;
}

void Body::MoveRight(float distance)
{
	location += distance * orientation.f1;
}

void Body::MoveUp(float distance)
{
	location += distance * orientation.f2;
}

void Body::MoveDown(float distance)
{
	location -= distance * orientation.f2;
}

void Body::Stop()
{
	velocity = { 0,0,0 };
	angularVelocity = { 0,0,0 };
}

void Body::TurnRight(float angle)
{
	orientation = Rotation(angle, orientation.f2) * orientation;
}

void Body::TurnLeft(float angle)
{
	orientation = Rotation(-angle, orientation.f2) * orientation;
}

void Body::TurnUp(float angle)
{
	orientation = Rotation(angle, orientation.f1) * orientation;
}

void Body::TurnDown(float angle)
{
	orientation = Rotation(-angle, orientation.f1) * orientation;
}

void Body::TurnClockwise(float angle)
{
	orientation = Rotation(-angle, orientation.f3) * orientation;
}

void Body::TurnAntiClockwise(float angle)
{
	orientation = Rotation(angle, orientation.f3) * orientation;
}

