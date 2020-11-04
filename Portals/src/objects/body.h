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
	float scale;
	Vec3D location;
	Mat_3D orientation;
	Vec3D velocity;
	float mass;
	Vec3D angularVelocity; // could use something like this in the future
//	float angularInertia; // to be more realistic, this should be a 3D tensor, but for now, it's just an isotropic scalar
//	Mat_3D angularInertia;
//	BodyType type; // this might not even be needed here.
};






