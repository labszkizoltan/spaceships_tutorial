
#include "ai.h"
#include "Portals/src/utils/Vector_3D.h"
#include "Portals/src/utils/Matrix_3D.h"

#include <limits>

#include "Portals/src/global_constants.h"

// AIAccelerationScript functions:
// typedef Vec3D(*AIAccelerationScript) (std::vector<Body>& bodies, float timestep, AI& ai);
Vec3D AIAccelerationFunction_basic(std::vector<Body>& bodies, float timestep, AI& ai)
{
	Vec3D dr = ai.m_TargetBody->location - ai.m_BodyPtr->location;
	Vec3D dv = ai.m_TargetBody->velocity - ai.m_BodyPtr->velocity;
	
	if (dr.length() > ai.m_Props.m_AttractionRange && dr.length() < 2.0f*ai.m_Props.m_AttractionRange)
	{
		return (ai.m_Props.m_Acceleration * timestep / dr.length()) * dr;
	}
	else if (dr.length() < ai.m_Props.m_RepulsionRange)
	{
		return -ai.m_Props.m_Acceleration * timestep / dr.length() * dr;
	}

	return Vec3D();
}

Vec3D AIAccelerationFunction_advanced(std::vector<Body>& bodies, float timestep, AI& ai)
{
	Vec3D dr = ai.m_TargetBody->location - ai.m_BodyPtr->location;
//	Vec3D ve = dr - dr * (ai.m_Props.m_RepulsionRange / dr.length()); // an extra velocity addon, to close the distance from the target
	Vec3D ve = dr*(1 - ai.m_Props.m_RepulsionRange / dr.length()); // an extra velocity addon, to close the distance from the target

	Vec3D dv = dr.length() > 3.0f*ai.m_Props.m_AttractionRange ? Vec3D() : ai.m_TargetBody->velocity + ve - ai.m_BodyPtr->velocity;

	return dv.lengthSquare()==0.0f ? Vec3D() : (ai.m_Props.m_Acceleration * timestep / dv.length() )*dv;
}

Vec3D AIAccelerationFunction_stationaryPlatform(std::vector<Body>& bodies, float timestep, AI& ai)
{
	return Vec3D();
}


// TODO
// create an accel function that drives the object into its target, causing collision damage


// ****************************** //
// ***** AI class functions ***** //
// ****************************** //

AI::AI() : m_BodyPtr(nullptr), m_TargetBody(nullptr), m_ShotTimer(0.0f), m_Props(AIProperties()) {}

AI::AI(Body* bodyPtr, Body* targetPtr, AIProperties props, uint32_t team) : m_BodyPtr(bodyPtr), m_TargetBody(targetPtr), m_ShotTimer(0.0f), m_Props(props), m_TeamID(team) {}


AI::~AI()
{
	m_BodyPtr = nullptr;
	m_TargetBody = nullptr;
}

void AI::Update(Scene& scene, float timestep, AIAccelerationScript accelFunc)
{
	int bodyIndex = ((int)m_BodyPtr - (int)&scene.m_Bodies[0]) / sizeof(Body);
	if (scene.m_Integrities[bodyIndex] <= 0.0f) { return; } // check if the body is still valid, or destroyed

	int targetBodyIndex = ((int)m_TargetBody - (int)&scene.m_Bodies[0]) / sizeof(Body);
	if (scene.m_Integrities[targetBodyIndex] <= 0.0f) { Retarget(scene); } // if the target is destroyed, search for another one

	Vec3D dr = m_TargetBody->location - m_BodyPtr->location;
	Vec3D z = m_BodyPtr->orientation.f3;

	Vec3D tempAcc = accelFunc(scene.m_Bodies, timestep, *this);
	m_BodyPtr->AccelerateDir(accelFunc(scene.m_Bodies, timestep, *this));

	// turn towards the target:
	Vec3D n = CrossProduct(z, dr); // no need to normalize the vector, since the Rotation matrix generator function does that
	float phi = acos(z*dr / (z.length()*dr.length()));
	phi = !std::isfinite(phi) ? 0.0f : (phi > (m_Props.m_TurnRate*timestep) ? (m_Props.m_TurnRate*timestep) : phi);
	m_BodyPtr->Turn(n, phi);

	// Shoot
	if (dr.length() < g_AIDefaultWeaponRange && m_ShotTimer > m_Props.m_ShotCooldown && phi < 0.05f) // PARAMETER AIShootPrecision
	{
		scene.OnShoot(m_BodyPtr, g_AIDefaultWeaponRange, g_TimeToLive); m_ShotTimer = 0.0f;
	}

	m_ShotTimer += timestep;
}

void AI::Retarget(Scene& scene)
{
	std::vector<std::pair<float,int>> distances_indices;
	distances_indices.reserve(scene.m_Integrities.size());

	int bodyIndex = ((int)m_BodyPtr - (int)&scene.m_Bodies[0]) / sizeof(Body);

	for (int i = 0; i < scene.m_Integrities.size(); i++)
	{
		if (scene.m_Integrities[i] > 0.0f && i != bodyIndex)
		{
			distances_indices.emplace_back(
				std::pair<float, int>((m_BodyPtr->location-scene.m_Bodies[i].location).lengthSquare(), i)
			);
		}
	}

	float min_dist = std::numeric_limits<float>::max();
	int corresponding_index = scene.m_Integrities.size();
	for (int i = 0; i < distances_indices.size(); i++)
	{
		if (distances_indices[i].first < min_dist)
		{
			min_dist = distances_indices[i].first;
			corresponding_index = distances_indices[i].second;
		}
	}

	m_TargetBody = &scene.m_Bodies[corresponding_index];
}


