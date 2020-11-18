#pragma once


// typedef void(*AccelerationFunction)(const std::vector<Body>& bodies, std::vector<Vec3D> accelerations);
void SimpleAcceleration(std::vector<Body>& bodies, std::vector<Vec3D>& accelerations, std::vector<Vec3D>& angular_accelerations)
{
	static float dragCoef = 0.1f;
	for (int i = 0; i < bodies.size(); i++)
	{
		accelerations[i] = CrossProduct({ 0,-0.5,0 }, bodies[i].velocity) - dragCoef * bodies[i].velocity; // +Vec3D((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
		angular_accelerations[i] = -0.1f*dragCoef*bodies[i].angularVelocity;
	}
}

// Gravity force
void GravityAcceleration(std::vector<Body>& bodies, std::vector<Vec3D>& accelerations, std::vector<Vec3D>& angular_accelerations)
{
	static float dragCoef = 0.01f;
	for (int i = 0; i < accelerations.size(); i++)
		accelerations[i] = { 0,0,0 };

	static float forceMinRange = 4.0f;
	static float forceStrenght = 1.0f;
	static float distanceSquare = 0.0f;
	static Vec3D forceDirection = { 0,0,0 };

	for (int i = 0; i < (bodies.size() - 1); i++)
	{
		for (int j = i + 1; j < bodies.size(); j++)
		{
			distanceSquare = (bodies[i].location - bodies[j].location).lengthSquare();
			forceDirection = bodies[i].location - bodies[j].location;
			accelerations[i] -= distanceSquare < forceMinRange ? Vec3D{ 0, 0, 0 } : forceStrenght * bodies[j].mass / distanceSquare * forceDirection / sqrt(distanceSquare);
			accelerations[j] += distanceSquare < forceMinRange ? Vec3D{ 0, 0, 0 } : forceStrenght * bodies[i].mass / distanceSquare * forceDirection / sqrt(distanceSquare);
		}
	}

	for (int i = 0; i < bodies.size(); i++)
	{
		accelerations[i] -= dragCoef * bodies[i].velocity / bodies[i].mass; // +Vec3D((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
		angular_accelerations[i] = -0.1f*dragCoef*bodies[i].angularVelocity;
	}
}

// Cutting corners, when a body has a mass less than the threshold, it doesnt pull on other objects
void SimplifiedGravity(std::vector<Body>& bodies, std::vector<Vec3D>& accelerations, std::vector<Vec3D>& angular_accelerations)
{
	static std::vector<int> massiveBodyIndices;
	static bool first_time = true;
	static float threshold_for_gravity = 500.0f;

	if (first_time)
	{
		for (int i = 0; i < bodies.size(); i++)
		{
			if (bodies[i].mass > threshold_for_gravity) { massiveBodyIndices.push_back(i); }
		}
		first_time = false;
	}

	static float dragCoef = 0.0f;
	for (int i = 0; i < accelerations.size(); i++)
		accelerations[i] = { 0,0,0 };

	static float forceMinRange = 4.0f;
	static float forceStrenght = 1.0f;
	static float distanceSquare = 0.0f;
	static Vec3D forceDirection = { 0,0,0 };

	for (int i = 0; i < bodies.size(); i++)
	{
		for (int j = 0; j < massiveBodyIndices.size(); j++)
		{
			distanceSquare = (bodies[i].location - bodies[massiveBodyIndices[j]].location).lengthSquare();
			forceDirection = bodies[i].location - bodies[massiveBodyIndices[j]].location;
			accelerations[i] -= distanceSquare < forceMinRange ? Vec3D{ 0, 0, 0 } : forceStrenght * bodies[massiveBodyIndices[j]].mass / distanceSquare * forceDirection / sqrt(distanceSquare);
		}
	}

	for (int i = 0; i < bodies.size(); i++)
	{
		accelerations[i] -= dragCoef * bodies[i].velocity / bodies[i].mass; // +Vec3D((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
		angular_accelerations[i] = -0.1f*dragCoef*bodies[i].angularVelocity;
	}
}

void NoAcceleration(std::vector<Body>& bodies, std::vector<Vec3D>& accelerations, std::vector<Vec3D>& angular_accelerations)
{
}






