#pragma once

#include "Portals/src/utils/Matrix_3D.h"

struct Observer
{
	Vec3D translation;
	Mat_3D orientation;
	float zoom_level;

	Observer();
	Observer(Vec3D v, Mat_3D m, float zoom);
	~Observer() = default;

	void MoveForward(float distance);
	void MoveBackward(float distance);
	void MoveLeft(float distance);
	void MoveRight(float distance);
	void MoveUp(float distance);
	void MoveDown(float distance);

	void TurnRight(float angle);
	void TurnLeft(float angle);
	void TurnUp(float angle);
	void TurnDown(float angle);
	void TurnClockwise(float angle);
	void TurnAntiClockwise(float angle);

	void ZoomIn(float multiplier);
	void ZoomOut(float multiplier);
};





