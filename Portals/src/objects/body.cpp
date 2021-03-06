
#include "body.h"

void Body::AccelerateDir(Vec3D deltav)
{
	velocity += deltav;
}

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

void Body::MitigateRotation(float mitigationFactor)
{
	angularVelocity /= mitigationFactor;
}

void Body::Turn(Vec3D axis, float angle)
{
//	This was put in for debugging
//	Mat_3D rotation = Rotation(angle, axis);
//	float* blah = &rotation.f1.x;
//	for (int i = 0; i < 9; i++)
//	{
//		if (!std::isfinite(blah[i]))
//		{
//			std::cout << "non finite value found! \n";
//		}
//	}
//	orientation = rotation * orientation;
	orientation = Rotation(angle, axis) * orientation;
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

