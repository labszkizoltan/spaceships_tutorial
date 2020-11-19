#pragma once

#include "Portals/src/utils/Vector_3D.h"
#include "Portals/src/utils/Matrix_3D.h"
#include "Portals/src/renderer/textured_mesh.h"


enum class BodyType
{
	cube_low_res, cube, cube_high_res,
	sphere_low_res, sphere, sphere_high_res
};


struct Body
{
	float scale;		// 1 float
	Vec3D location;		// 3 float
	Mat_3D orientation;	// 9 float
	Vec3D velocity;
	float mass;
	Vec3D angularVelocity; // could use something like this in the future
//	float angularInertia; // to be more realistic, this should be a 3D tensor, but for now, it's just an isotropic scalar
//	Mat_3D angularInertia;
//	BodyType type; // this might not even be needed here.

	void AccelerateDir(Vec3D deltav);
	void Accelerate(float deltav);
	void Deccelerate(float deltav);
	void MoveForward(float distance);
	void MoveBackward(float distance);
	void MoveLeft(float distance);
	void MoveRight(float distance);
	void MoveUp(float distance);
	void MoveDown(float distance);

	void Stop();

	void Turn(Vec3D axis, float angle);
	void TurnRight(float angle);
	void TurnLeft(float angle);
	void TurnUp(float angle);
	void TurnDown(float angle);
	void TurnClockwise(float angle);
	void TurnAntiClockwise(float angle);


};






