#pragma once

#include "Portals/src/utils/Vector_3D.h"
#include "Portals/src/utils/Matrix_3D.h"
#include "Portals/src/renderer/textured_mesh.h"




struct body
{
	Vec3D location;
	Mat_3D orientation;
	float scale;
	Vec3D velocity;
//	Vec3D angularVelocity; // could use something like this in the future
	float mass;
//	Mat_3D angularMomentum;
	BodyType type;
};






